#include "pch.h"
#include "ObjectManager.h"
#include "ProjectileManager.h"
#include "Level.h"
#include "PowerUp.h"
#include "Camera.h"
#include <fstream>
#include <string>
#include "Item.h"

// Enemy Types
#include "HotHead.h"
#include "Enemy.h"
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

// TODO: Implement removal puffs for enemies & items

ObjectManager* ObjectManager::m_pObjectManager{ nullptr };

ObjectManager::ObjectManager(Kirby* pKirby)
	: m_pKirby{ pKirby }
{
	if (m_pObjectManager)
	{
		delete m_pObjectManager;
	}
	m_pObjectManager = this;

	m_pKirby->SetObjectManager(this);
}

ObjectManager::~ObjectManager()
{
	DeleteEnemies();
	DeleteItems();
	DeleteFXs();
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
		if (pItem->IsRemoved() == false && pItem->NeedsToBeDrawn())
		{
			pItem->Draw();
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

void ObjectManager::Update(float elapsedSec, const Rectf& visibleArea)
{
	m_VisibleArea = visibleArea;

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
	for (Enemy* pEnemy : m_pEnemies)
	{
		pEnemy->InitializePowerUp();
	}
}

bool ObjectManager::InsideXScreenBounds(const Rectf& shape)
{
	const float viewExtension{ 0.f };
	return m_VisibleArea.left < shape.left + shape.width + viewExtension
		&& shape.left < m_VisibleArea.left + m_VisibleArea.width + viewExtension;
}

bool ObjectManager::InsideYScreenBounds(const Rectf& shape)
{
	const float viewExtension{ 0.f };
	return m_VisibleArea.bottom < shape.bottom + shape.height + viewExtension
		&& shape.bottom < m_VisibleArea.bottom + m_VisibleArea.height + viewExtension;
}

void ObjectManager::UpdateEnemy(Enemy*& pEnemy, float elapsedSec)
{
	Rectf enemyShape{ pEnemy->GetShape() };
	const bool insideXScreenBounds{ InsideXScreenBounds(enemyShape) }, insideYScreenBounds{ InsideYScreenBounds(enemyShape) };
	if (pEnemy->IsActive())
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

		CheckEnemyRemovalConditions(pEnemy, insideXScreenBounds, insideYScreenBounds);
	}
	else if (pEnemy->HasBeenOffScreen() && insideXScreenBounds && insideYScreenBounds)
	{
		pEnemy->SetActivity(true);
	}
	else if ( pEnemy->HasBeenOffScreen() == false && (insideXScreenBounds == false || insideYScreenBounds == false))
	{
		float xDirection{ pEnemy->GetShape().left - m_pKirby->GetShape().left > 0.f ? -1.f : 1.f };
		pEnemy->SetOffScreen(true, xDirection);
	}
}

void ObjectManager::CheckEnemyRemovalConditions(Enemy*& pEnemy, bool insideXScreenBounds, bool insideYScreenBounds)
{
	// CONDITIONS AND VARIABLES
	const bool hasCollidedWithKirby{ pEnemy->IsActive() && m_pKirby->CheckCollisionWith(pEnemy) };
	const bool outsideExtendedViewingArea{ !insideXScreenBounds || !insideYScreenBounds };
	const bool fellOutOfWorld{ pEnemy->GetShape().bottom + pEnemy->GetShape().height < -100.f };
	const bool hasPassedKirby{ m_pKirby->GetDirection() > 0.f ? m_pKirby->GetShape().left > pEnemy->GetShape().left : m_pKirby->GetShape().left < pEnemy->GetShape().left };

	// ACTUAL CODE

	if (ProjectileManager::GetProjectileMngr()->ProjectileHasHit(pEnemy, Projectile::ActorType::enemy) || fellOutOfWorld || outsideExtendedViewingArea)
	{
		AddRemovalFX(pEnemy->GetLocation(), RemovalFX::FXType::enemy);
		pEnemy->Reset();
	}
	else if (pEnemy->IsBeingInhaled())
	{
		if (hasPassedKirby)
		{
			m_pKirby->SetBloated();
			if (pEnemy->HasPowerUp())
			{
				pEnemy->TransferPowerUp(m_pKirby);
			}
			pEnemy->Reset();
		}
	}
	else if (hasCollidedWithKirby)
	{
		float bounceDirection{ pEnemy->GetShape().left < m_pKirby->GetShape().left ? 1.f : -1.f };
		m_pKirby->DecrementHealth(bounceDirection);
		AddRemovalFX(pEnemy->GetLocation(), RemovalFX::FXType::enemy);
		pEnemy->Reset();
	}
}



void ObjectManager::UpdateItem(Item*& pItem, float elapsedSec)
{
	// If the item is gone, the game should no longer update the item
	if (pItem->IsRemoved())
	{
		return;
	}

	Rectf itemShape{ pItem->GetShape() };
	utils::HitInfo hitInfo;
	bool isVerticalCollision;

	const bool hasCollidedWithKirby{ m_pKirby->CheckCollisionWith(pItem, hitInfo, isVerticalCollision)};
	const bool insideXScreenBounds{ InsideXScreenBounds(itemShape) };
	const bool insideYScreenBounds{ InsideYScreenBounds(itemShape) };
	const bool outsideExtendedViewingArea{ !insideXScreenBounds || !insideYScreenBounds };
	const bool fellOutOfWorld{ pItem->GetShape().bottom < -50.f };

	if (pItem->IsInhalable())
	{
		pItem->ToggleBeingInhaled(m_pKirby->GetInhalationZone());
	}
	if (pItem->IsBeingInhaled())
	{
		DoItemInhalationActions(pItem);
	}
	else if (pItem->IsDestructible() && ProjectileManager::GetProjectileMngr()->ProjectileHasHit(pItem, Projectile::ActorType::enemy))
	{
		pItem->Remove();
		AddRemovalFX(pItem->GetLocation(), RemovalFX::FXType::item);
	}
	else if (hasCollidedWithKirby && pItem->IsSolid())
	{
		DoItemCollision(pItem, isVerticalCollision, hitInfo);
	}
	else if (hasCollidedWithKirby && pItem->HasCollisionEvent())
	{
		pItem->DoCollisionEvent();
		pItem->Remove();
	}
	else if( fellOutOfWorld || (outsideExtendedViewingArea && pItem->IsPersistent() == false))
	{
		pItem->Remove();
	}
	
	if (pItem->IsRemoved() == false)
	{
		pItem->Update(elapsedSec);
	}
}

void ObjectManager::UpdateRemovalFXs(float elapsedSec)
{
	int idx{};
	if (m_pRemovalFX.size() > 0)
	{
		while (idx < m_pRemovalFX.size())
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

void ObjectManager::DoItemInhalationActions(Item*& pItem)
{
	const bool hasPassedKirby{ m_pKirby->GetDirection() > 0.f ? m_pKirby->GetShape().left > pItem->GetShape().left : m_pKirby->GetShape().left < pItem->GetShape().left };
	if (hasPassedKirby)
	{
		if (pItem->IsBloatItem())
		{
			m_pKirby->SetBloated();
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

void ObjectManager::DoItemCollision(Item*& pItem, bool isVerticalCollision, const utils::HitInfo& hitInfo)
{
	if (isVerticalCollision)
	{
		if (hitInfo.lambda < 0.5f)
		{
			// TODO: fix stuttering bug when on star block
			m_pKirby->SetLocation(Point2f{ m_pKirby->GetShape().left, pItem->GetShape().bottom + pItem->GetShape().height});
			m_pKirby->ForceIsOnGround();
			m_pKirby->SetVerticalVelocityToZero();
		}
		else
		{
			float fallPixel{ 1.f }; // Pixel offset so that kirby doesn't stick to roof
			m_pKirby->SetLocation(Point2f{ m_pKirby->GetShape().left, pItem->GetShape().bottom - m_pKirby->GetShape().height - fallPixel });
			m_pKirby->SetVerticalVelocityToZero();
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
	for (Item* pItem : m_pItems)
	{
		delete pItem;
	}
	m_pItems.clear();
}

void ObjectManager::LoadObjectsByLevelName(const std::string& levelName)
{
	std::string filePath{ "resources/enemydata/" + levelName + ".txt" };
	LoadObjectsFromFile(filePath);

	SetEnemyProjectileManagerPointers();
}

//void ObjectManager::ResetEnemies()
//{
//	for (Enemy* pEnemy : m_pEnemies)
//	{
//		pEnemy->Reset();
//		pEnemy->SetActivity(true);
//	}
//}

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
		m_pEnemies.push_back(new WaddleDee{ location });
	}
	else if (objectName == "brontoburt")
	{
		m_pEnemies.push_back(new BrontoBurt{ location });
	}
	else if (objectName == "hothead")
	{
		m_pEnemies.push_back(new HotHead{ location });
	}
	else if (objectName == "sparky")
	{
		m_pEnemies.push_back(new Sparky{ location });
	}
	else if (objectName == "bounder")
	{
		m_pEnemies.push_back(new Bounder{ location });
	}
	else if (objectName == "laserball")
	{
		m_pEnemies.push_back(new LaserBall{ location });
	}
	else if (objectName == "rocky")
	{
		m_pEnemies.push_back(new Rocky{ location });
	}
	else if (objectName == "parasol")
	{
		m_pEnemies.push_back(new Parasol{ location });
	}
	else if (objectName == "starblock")
	{
		m_pItems.push_back(new StarBlock{ location });
	}
	else if (objectName == "oneup")
	{
		m_pItems.push_back(new OneUp{ location , m_pKirby});
	}
	else if (objectName == "healthbug")
	{
		m_pItems.push_back(new HealthBug{ location, m_pKirby });
	}
}
