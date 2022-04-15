#include "pch.h"
#include "utils.h"
#include "ObjectManager.h"
#include "ProjectileManager.h"
#include "Level.h"
#include "PowerUp.h"
#include "Camera.h"
#include <fstream>
#include <string>
#include "Item.h"
#include "PowerStar.h"

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

ObjectManager::ObjectManager(Kirby* pKirby)
	: m_pKirby{ pKirby }
	, m_pProjectileManager{ new ProjectileManager{} }
{
	m_pKirby->SetProjectileManager(m_pProjectileManager);
}

ObjectManager::~ObjectManager()
{
	DeleteEnemies();
	DeleteItems();

	delete m_pProjectileManager;
}

void ObjectManager::Draw() const
{
	for (Enemy* pEnemy : m_pEnemies)
	if (pEnemy->IsActive())
	{
		pEnemy->Draw();
	}

	for (Item* pItem : m_pItems)
	{
		if (pItem->IsRemoved() == false && pItem->NeedsToBeDrawn())
		{
			pItem->Draw();
		}
	}

	m_pProjectileManager->Draw();
}

void ObjectManager::Update(float elapsedSec, const Rectf& visibleArea)
{
	m_VisibleArea = visibleArea;

	m_pProjectileManager->Update(elapsedSec);

	UpdateEnemies(elapsedSec);
	UpdateItems(elapsedSec);
	
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
		pEnemy->SetProjectileManager(m_pProjectileManager);
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

void ObjectManager::SetLevelPointers(Level* levelPtr)
{
	for (Enemy* pEnemy : m_pEnemies)
	{
		pEnemy->SetCurrentLevel(levelPtr);
	}

	for (Item* pItem : m_pItems)
	{
		pItem->SetCurrentLevel(levelPtr);
	}
}

void ObjectManager::UpdateEnemy(Enemy*& pEnemy, float elapsedSec)
{
	Rectf enemyShape{ pEnemy->GetShape() };
	const bool insideXScreenBounds{ InsideXScreenBounds(enemyShape) }, insideYScreenBounds{ InsideYScreenBounds(enemyShape) };
	if (pEnemy->IsActive())
	{
		pEnemy->DoAbilityCheck(m_pKirby);

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

	if (m_pProjectileManager->ProjectileHasHit(pEnemy, Projectile::ActorType::enemy) || fellOutOfWorld || outsideExtendedViewingArea)
	{
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

	const bool hasCollidedWithKirby{ m_pKirby->CheckCollisionWith(pItem)};
	const bool insideXScreenBounds{ InsideXScreenBounds(itemShape) };
	const bool insideYScreenBounds{ InsideYScreenBounds(itemShape) };
	const bool outsideExtendedViewingArea{ !insideXScreenBounds || !insideYScreenBounds };
	const bool fellOutOfWorld{ pItem->GetShape().bottom < -50.f };
	const bool hasPassedKirby{ m_pKirby->GetDirection() > 0.f ? m_pKirby->GetShape().left > pItem->GetShape().left : m_pKirby->GetShape().left < pItem->GetShape().left };

	if (hasCollidedWithKirby && pItem->HasCollisionEvent())
	{
		pItem->DoCollisionEvent();
		pItem->Remove();
	}
	else if( fellOutOfWorld || (outsideExtendedViewingArea && pItem->IsPersistent() == false))
	{
		pItem->Remove();
	}

	if (pItem->IsInhalable())
	{
		pItem->ToggleBeingInhaled(m_pKirby->GetInhalationZone());
	}
	
	if (pItem->IsBeingInhaled())
	{
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
			pItem->Remove();
		}
		pItem->SetInhalationVelocities(m_pKirby->GetShape());
	}
	pItem->Update(elapsedSec);
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

void ObjectManager::LoadEnemiesByLevelName(const std::string& levelName)
{
	std::string filePath{ "resources/enemydata/" + levelName + ".txt" };
	LoadEnemiesFromFile(filePath);

	SetEnemyProjectileManagerPointers();
}

void ObjectManager::ResetEnemies()
{
	for (Enemy* pEnemy : m_pEnemies)
	{
		pEnemy->Reset();
		pEnemy->SetActivity(true);
	}
}

void ObjectManager::LoadEnemiesFromFile(const std::string& filePath)
{
	std::fstream file;
	file.open(filePath, std::ios::in);

	if (!file)
	{
		std::cout << "COULD NOT FIND ENEMY FILE \n";
		return;
	}

	while (file.good())
	{
		std::string bufferString;
		std::getline(file, bufferString, '\n');
		
		const int nrOfPositions{ 3 };
		int splitPositions[nrOfPositions]{};
		splitPositions[0] = int(bufferString.find(','));
		splitPositions[1] = int(bufferString.find(',', splitPositions[0] + 1));
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

		CreateEnemy(enemyName, location);
	}
}

void ObjectManager::CreateEnemy(const std::string& enemyName, const Point2f& location)
{
	if (enemyName == "waddledee")
	{
		m_pEnemies.push_back(new WaddleDee{ location });
	}
	else if (enemyName == "brontoburt")
	{
		m_pEnemies.push_back(new BrontoBurt{ location });
	}
	else if (enemyName == "hothead")
	{
		m_pEnemies.push_back(new HotHead{ location });
	}
	else if (enemyName == "sparky")
	{
		m_pEnemies.push_back(new Sparky{ location });
	}
	else if (enemyName == "bounder")
	{
		m_pEnemies.push_back(new Bounder{ location });
	}
	else if (enemyName == "laserball")
	{
		m_pEnemies.push_back(new LaserBall{ location });
	}
	else if (enemyName == "rocky")
	{
		m_pEnemies.push_back(new Rocky{ location });
	}
	else if (enemyName == "parasol")
	{
		m_pEnemies.push_back(new Parasol{ location });
	}
}
