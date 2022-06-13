#include "pch.h"
#include "ObjectManager.h"
#include "ProjectileManager.h"
#include "PowerUp.h"
#include "Camera.h"
#include <fstream>


// Enemy Types
#include "HotHead.h"
#include "WaddleDee.h"
#include "Rocky.h"
#include "BrontoBurt.h"
#include "Bounder.h"
#include "LaserBall.h"
#include "Sparky.h"
#include "Parasol.h"

// Item Types
#include "PowerStar.h"
#include "OneUp.h"
#include "HealthBug.h"
#include "StarBlock.h"

// Boss
#include "MrTickTock.h"

ObjectManager* ObjectManager::m_pObjectManager{ nullptr };

ObjectManager::ObjectManager(Kirby* pKirby, LevelManager* pLevelManager, ProjectileManager* pProjectileManager)
	: m_pKirby{ pKirby }
	, m_pLevelManager{pLevelManager}
	, m_pProjectileManager{pProjectileManager}
	, m_pMrTickTock{nullptr}
{
	if (m_pObjectManager)
	{
		delete m_pObjectManager;
	}
	m_pObjectManager = this;
}

ObjectManager::~ObjectManager()
{
	DeleteEnemies();
	DeleteItems();
	DeleteFXs();
	if (m_pMrTickTock)
	{
		delete m_pMrTickTock;
	}
}

ObjectManager* ObjectManager::GetObjectMngr()
{
	return m_pObjectManager;
}

void ObjectManager::Draw() const
{
	DrawFXs();
	DrawItems();
	DrawEnemies();

	if (m_pMrTickTock)
	{
		m_pMrTickTock->Draw();
	}
}

void ObjectManager::DrawEnemies() const
{
	for (Enemy* pEnemy : m_pEnemies)
	{
		if (pEnemy->IsActive())
		{
			pEnemy->Draw();
		}
	}
}

void ObjectManager::DrawItems() const
{
	for (Item* pItem : m_pItems)
	{
		if (pItem) {
			if (pItem->IsRemoved() == false && pItem->NeedsToBeDrawn())
			{
				pItem->Draw();
			}
		}
	}
}

void ObjectManager::DrawFXs() const
{
	for (RemovalFX* pFX : m_pRemovalFX)
	{
		pFX->Draw();
	}
}

void ObjectManager::Update(float elapsedSec, Camera* pCamera)
{
	m_VisibleArea = pCamera->GetVisibleArea();

	UpdateTickTock(elapsedSec, pCamera);
	UpdateEnemies(elapsedSec);
	UpdateItems(elapsedSec);
	UpdateRemovalFXs(elapsedSec);
	
}

void ObjectManager::UpdateEnemies(float elapsedSec)
{
	for (Enemy*& pEnemy : m_pEnemies)
	{
		if (pEnemy)
		{
			UpdateEnemy(pEnemy, elapsedSec);
		}
	}
}

void ObjectManager::UpdateItems(float elapsedSec)
{
	for (Item*& pItem : m_pItems)
	{
		if (pItem)
		{
			UpdateItem(pItem, elapsedSec);
		}
	}
}

void ObjectManager::SetEnemyProjectileManagerPointers()
{
	for (Enemy*& pEnemy : m_pEnemies)
	{
		pEnemy->InitializePowerUp();
	}
}

void ObjectManager::DoCameraFunctionality(Camera* pCamera)
{
	if (m_pMrTickTock->IsDead() || m_pKirby->GetHealth() == 0)
	{
		if (pCamera->IsLocked())
		{
			pCamera->Unlock();
		}
		return;
	}

	Point2f bossAreaCenter{ 310.f, 56.f };
	if (!pCamera->IsLocked() && utils::IsPointInRect(bossAreaCenter, m_VisibleArea))
	{
		pCamera->SetLocked(bossAreaCenter);
	}
}

void ObjectManager::DoInteractionsWithKirby()
{
	if (m_pProjectileManager->ProjectileHasHit(m_pMrTickTock))
	{
		m_pMrTickTock->DecrementHealth();
	}
	else if (m_pKirby->CheckCollisionWith(m_pMrTickTock))
	{
		float bounceDirection{ m_pMrTickTock->GetShape().left < m_pMrTickTock->GetShape().left ? 1.f : -1.f };
		if (m_pKirby->HasPowerUp())
		{
			const float velocityTreshold{ -5.f };
			if ((m_pKirby->GetVelocity().y < velocityTreshold) && (m_pKirby->GetPowerUp()->GetType() == PowerUp::PowerUpType::stone))
			{
				m_pMrTickTock->DecrementHealth();
			}
			else
			{
				m_pKirby->DecrementHealth(bounceDirection);
			}
		}
		else
		{
			m_pKirby->DecrementHealth(bounceDirection);
		}
	}
}

void ObjectManager::UpdateEnemy(Enemy*& pEnemy, float elapsedSec)
{
	Rectf enemyShape{ pEnemy->GetShape() };
	if (pEnemy->IsActive())
	{
		DoEnemyActiveUpdate(pEnemy, elapsedSec);
	}
	else if (pEnemy->HasBeenOffScreen() && pEnemy->IsOnScreen())
	{
		float xDirection{ pEnemy->GetShape().left - m_pKirby->GetShape().left > 0.f ? -1.f : 1.f };
		pEnemy->SetDirection(xDirection);
		pEnemy->SetActivity(true);
	}
	else if ( pEnemy->HasBeenOffScreen() == false && !pEnemy->IsOnScreen())
	{
		pEnemy->SetOffScreen(true);
	}
}

void ObjectManager::DoEnemyActiveUpdate(Enemy* pEnemy, float elapsedSec)
{
	pEnemy->DoChecksOnKirby(m_pKirby);

	if (pEnemy->IsInhalable())
	{
		pEnemy->ToggleBeingInhaled(m_pKirby->GetInhalationZone());
	}

	if (pEnemy->IsBeingInhaled())
	{
		pEnemy->SetInhalationVelocities(m_pKirby->GetShape());
	}

	pEnemy->Update(elapsedSec);

	CheckEnemyRemovalConditions(pEnemy);
}

void ObjectManager::CheckEnemyRemovalConditions(Enemy*& pEnemy)
{
	// CONDITIONS AND VARIABLES
	const bool hasCollidedWithKirby{ pEnemy->IsActive() && m_pKirby->CheckCollisionWith(pEnemy) };
	const bool fellOutOfWorld{ pEnemy->GetShape().bottom + pEnemy->GetShape().height < -100.f };
	const bool hasPassedKirby{ m_pKirby->GetDirection() > 0.f ? m_pKirby->GetShape().left > pEnemy->GetShape().left : m_pKirby->GetShape().left < pEnemy->GetShape().left };

	// ACTUAL CODE
	const bool projectileHasHit{ m_pProjectileManager->ProjectileHasHit(pEnemy) };
	if (projectileHasHit || fellOutOfWorld || !pEnemy->IsOnScreen())
	{
		int scoreReward{ 0 };
		if (projectileHasHit)
		{
			SoundFXManager::Play("damage2");
			scoreReward = 400;
		}
		KillEnemy(pEnemy, true, scoreReward);
	}
	else if (pEnemy->IsBeingInhaled())
	{
		if (hasPassedKirby)
		{
			DoEnemyAbsorption(pEnemy);
		}
	}
	else if (hasCollidedWithKirby)
	{
		DoEnemyKirbyCollision(pEnemy);
		if (m_pKirby->IsInvulnerable())
		{
			SoundFXManager::Play("damage2");
		}
	}
}

void ObjectManager::DoEnemyAbsorption(Enemy* pEnemy)
{
	m_pKirby->SetBloated();
	if (pEnemy->HasPowerUp())
	{
		pEnemy->TransferPowerUp(m_pKirby);
	}
	const int scoreReward{ 250 };

	KillEnemy(pEnemy, false, scoreReward);
}

void ObjectManager::DoEnemyKirbyCollision(Enemy* pEnemy)
{
	const int scoreReward{ 500 };
	float bounceDirection{ m_pKirby->GetRelativeDirection(pEnemy) };
	m_pKirby->DecrementHealth(bounceDirection);
	KillEnemy(pEnemy, true, scoreReward);
}

void ObjectManager::KillEnemy(Enemy* pEnemy, bool addRemovalFX, int scoreReward)
{
	if (addRemovalFX)
	{
		AddRemovalFX(pEnemy->GetLocation(), RemovalFX::FXType::enemy);
	}
	pEnemy->Reset();
	m_pKirby->AddScore(scoreReward);
}

void ObjectManager::UpdateItem(Item*& pItem, float elapsedSec)
{
	// If the item is gone, the game should no longer update the item
	if(pItem->IsRemoved())
	{
		return;
	}
	pItem->Update(elapsedSec);

	if (pItem)
	{
		if (pItem->IsInhalable())
		{
			pItem->ToggleBeingInhaled(m_pKirby->GetInhalationZone());
		}

		DoItemActions(pItem);
	}
}

void ObjectManager::DoItemActions(Item* pItem)
{
	utils::HitInfo hitInfo;
	bool isVerticalCollision;

	const bool hasCollidedWithKirby{ m_pKirby->CheckCollisionWith(pItem, hitInfo, isVerticalCollision) };
	const bool fellOutOfWorld{ pItem->GetShape().bottom < -50.f };

	if (pItem->IsBeingInhaled())
	{
		DoItemInhalationActions(pItem);
	}
	else if (pItem->IsDestructible())
	{
		if (m_pProjectileManager->ProjectileHasHit(pItem))
		{
			DoItemRemovalByProjectile(pItem);
		}
	}
	else if (hasCollidedWithKirby && pItem->IsSolid())
	{
		DoSolidCollision(pItem, isVerticalCollision, hitInfo);
	}
	else if (hasCollidedWithKirby && pItem->HasCollisionEvent())
	{
		DoItemCollisionEvent(pItem);
	}
	else if (fellOutOfWorld || (!pItem->IsOnScreen() && pItem->IsPersistent() == false))
	{
		pItem->Remove();
	}
}

void ObjectManager::DoItemCollisionEvent(Item* pItem)
{
	pItem->DoCollisionEvent();
	if (pItem->IsRemovedOnCollision())
	{
		pItem->Remove();
	}
}

void ObjectManager::DoItemRemovalByProjectile(Item* pItem)
{
	pItem->Remove();
	AddRemovalFX(pItem->GetLocation(), RemovalFX::FXType::item);
	m_pKirby->AddScore(10);
}

void ObjectManager::UpdateRemovalFXs(float elapsedSec)
{
	int idx{};
	if (int(m_pRemovalFX.size()) > 0)
	{
		while (idx < int(m_pRemovalFX.size()))
		{
			if (m_pRemovalFX[idx])
			{
				m_pRemovalFX[idx]->Update(elapsedSec);

				if (m_pRemovalFX[idx]->IsReadyToDestroy())
				{
					delete m_pRemovalFX[idx];
					m_pRemovalFX[idx] = nullptr;
					std::swap(m_pRemovalFX[idx], m_pRemovalFX[m_pRemovalFX.size() - 1]);
					m_pRemovalFX.pop_back();
				}
			}
			++idx;
		}
	}
}

void ObjectManager::UpdateTickTock(float elapsedSec, Camera* pCamera)
{
	if (m_pMrTickTock)
	{
		if (m_pMrTickTock->IsOnScreen())
		{
			m_pMrTickTock->Update(elapsedSec);
		}

		DoCameraFunctionality(pCamera);

		if (m_pMrTickTock->IsDead())
		{
			return;
		}

		DoInteractionsWithKirby();
	}
}

void ObjectManager::DoItemInhalationActions(Item*& pItem)
{
	const bool hasPassedKirby{ m_pKirby->GetDirection() > 0.f ? m_pKirby->GetShape().left > pItem->GetShape().left : m_pKirby->GetShape().left < pItem->GetShape().left };
	if (hasPassedKirby)
	{
		if (pItem->IsBloatItem())
		{
			m_pKirby->SetBloated();
			m_pKirby->AddScore(10);
		}
		if (pItem->HasPowerUp())
		{
			pItem->TransferPowerUp(m_pKirby);
		}
		if (pItem->HasCollisionEvent())
		{
			pItem->DoCollisionEvent();
		}
		pItem->Remove();
	}
	pItem->SetInhalationVelocities(m_pKirby->GetShape());
}

void ObjectManager::DoSolidCollision(Item*& pItem, bool isVerticalCollision, const utils::HitInfo& hitInfo)
{
	if (isVerticalCollision)
	{
		if (hitInfo.lambda < 0.5f)
		{
			// Haven't been able to fix stutter bug
			m_pKirby->SetLocation(Point2f{ m_pKirby->GetShape().left, pItem->GetShape().bottom + pItem->GetShape().height});
			m_pKirby->ForceIsOnGround();
			m_pKirby->SetVelocity(m_pKirby->GetVelocity().x, 0.f);
		}
		else
		{
			float fallPixel{ 1.f }; // Pixel offset so that kirby doesn't stick to roof
			m_pKirby->SetLocation(Point2f{ m_pKirby->GetShape().left, pItem->GetShape().bottom - m_pKirby->GetShape().height - fallPixel });
			m_pKirby->SetVelocity(m_pKirby->GetVelocity().x, 0.f);
		}
	}
	else
	{
		if (hitInfo.lambda < 0.5f)
		{
			m_pKirby->SetLocation(Point2f{ pItem->GetShape().left + pItem->GetShape().width, m_pKirby->GetShape().bottom });
		}
		else
		{
			m_pKirby->SetLocation(Point2f{ pItem->GetShape().left - m_pKirby->GetShape().width, m_pKirby->GetShape().bottom });
		}
	}
}

void ObjectManager::DeleteEnemies()
{
	for (Enemy* pEnemy : m_pEnemies)
	{
		delete pEnemy;
	}
	m_pEnemies.clear();
}

void ObjectManager::DeleteItems()
{
	for (Item* pItem : m_pItems)
	{
		delete pItem;
	}
	m_pItems.clear();
}

void ObjectManager::DeleteFXs()
{
	for (RemovalFX* pFX : m_pRemovalFX)
	{
		delete pFX;
	}
	m_pRemovalFX.clear();

}

void ObjectManager::AddRemovalFX(const Point2f& location, RemovalFX::FXType type)
{
	m_pRemovalFX.push_back(new RemovalFX{ location , type });
}

void ObjectManager::AddItem(Item* pItem)
{
	m_pItems.push_back(pItem);
}

void ObjectManager::ClearEnemyVector()
{
	for (Enemy* pEnemy : m_pEnemies)
	{
		delete pEnemy;
	}
	m_pEnemies.clear();
}

void ObjectManager::ClearObjectVector()
{
	if (m_pItems.size() > 0) return;

	for (Item* pItem : m_pItems)
	{
		delete pItem;
	}
	m_pItems.clear();
}

void ObjectManager::ClearFXVector()
{
	for (RemovalFX* pFX : m_pRemovalFX)
	{
		delete pFX;
	}
	m_pRemovalFX.clear();
}

void ObjectManager::LoadObjectsByLevelName(const std::string& levelName)
{
	std::string filePath{ "resources/enemydata/" + levelName + ".txt" };
	LoadObjectsFromFile(filePath);

	SetEnemyProjectileManagerPointers();
}

void ObjectManager::Clear()
{
	ClearObjectVector();
	ClearEnemyVector();
	ClearFXVector();

	if (m_pMrTickTock)
	{
		delete m_pMrTickTock;
		m_pMrTickTock = nullptr;
	}
}

Rectf ObjectManager::GetVisibleArea() const
{
	return m_VisibleArea;
}

void ObjectManager::LoadObjectsFromFile(const std::string& filePath)
{
	std::fstream file;
	file.open(filePath, std::ios::in);

	if (!file)
	{
		std::cout << "COULD NOT FIND OBJECT FILE \n";
		return;
	}

	while (file.good())
	{
		std::string bufferString;
		std::getline(file, bufferString, '\n');
		
		const int nrOfPositions{ 3 };
		int splitPositions[nrOfPositions]{};
		splitPositions[0] = int(bufferString.find(','));
		splitPositions[1] = int(bufferString.find(',', int(splitPositions[0] + 1)));
		splitPositions[2] = int(bufferString.size());

		// Safety check
		for (int idx{}; idx < nrOfPositions; ++idx)
		{
			if (splitPositions[idx] == -1)
			{
				return;
			}
		}
		
		std::string enemyName{ bufferString.substr(0, splitPositions[0])};
		float xCoordinate{ std::stof(bufferString.substr(splitPositions[0] + 1, splitPositions[1]))};
		float yCoordinate{ std::stof(bufferString.substr(splitPositions[1] + 1, splitPositions[2])) };
		Point2f location{ xCoordinate, yCoordinate };

		CreateObject(enemyName, location);
	}
}

void ObjectManager::CreateObject(const std::string& objectName, const Point2f& location)
{
	if (objectName == "waddledee")
	{
		m_pEnemies.push_back(new WaddleDee{ location, m_pLevelManager, m_pProjectileManager });
	}
	else if (objectName == "brontoburt")
	{
		m_pEnemies.push_back(new BrontoBurt{ location, m_pLevelManager, m_pProjectileManager });
	}
	else if (objectName == "hothead")
	{
		m_pEnemies.push_back(new HotHead{ location, m_pLevelManager, m_pProjectileManager });
	}
	else if (objectName == "sparky")
	{
		m_pEnemies.push_back(new Sparky{ location, m_pLevelManager, m_pProjectileManager });
	}
	else if (objectName == "bounder")
	{
		m_pEnemies.push_back(new Bounder{ location, m_pLevelManager, m_pProjectileManager });
	}
	else if (objectName == "laserball")
	{
		m_pEnemies.push_back(new LaserBall{ location, m_pLevelManager, m_pProjectileManager });
	}
	else if (objectName == "rocky")
	{
		m_pEnemies.push_back(new Rocky{ location, m_pLevelManager, m_pProjectileManager });
	}
	else if (objectName == "parasol")
	{
		m_pEnemies.push_back(new Parasol{ location, m_pLevelManager, m_pProjectileManager });
	}
	else if (objectName == "starblock")
	{
		m_pItems.push_back(new StarBlock{ location, m_pKirby, m_pLevelManager, m_pProjectileManager});
	}
	else if (objectName == "oneup")
	{
		m_pItems.push_back(new OneUp{ location , m_pKirby, m_pLevelManager, m_pProjectileManager});
	}
	else if (objectName == "healthbug")
	{
		m_pItems.push_back(new HealthBug{ location, m_pKirby, m_pLevelManager, m_pProjectileManager });
	}
	else if (objectName == "mrticktock")
	{
		if (m_pMrTickTock)
		{
			delete m_pMrTickTock;
		}
		m_pMrTickTock = new MrTickTock{ location, m_pKirby, m_pLevelManager, m_pProjectileManager};
	}
}
