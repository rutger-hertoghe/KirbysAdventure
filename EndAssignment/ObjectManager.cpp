#include "pch.h"
#include "ObjectManager.h"
#include "ProjectileManager.h"
#include "Enemy.h"
#include "WaddleDee.h"
#include "Level.h"
#include "HotHead.h"
#include "PowerUp.h"
#include "BrontoBurt.h"
#include "Camera.h"
#include "Sparky.h"
#include <fstream>
#include <string>
#include "Item.h"
#include "PowerStar.h"

ObjectManager::ObjectManager()
	: m_pKirby{ nullptr }
	, m_pProjectileManager{ new ProjectileManager{} }
{
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
		if (pItem->IsRemoved() == false)
		{
			pItem->Draw();
		}
	}

	m_pProjectileManager->Draw();
}

void ObjectManager::Update(float elapsedSec, const Point2f& cameraLocation)
{
	m_ViewLocation = cameraLocation;

	m_pProjectileManager->Update(elapsedSec);

	for (Enemy*& pEnemy : m_pEnemies)
	{
		if (pEnemy)
		{
			UpdateEnemy(pEnemy, elapsedSec);
		}
	}

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

void ObjectManager::LinkKirby(Kirby* kirbyPtr)
{
	m_pKirby = kirbyPtr;
	m_pKirby->SetProjectileManager(m_pProjectileManager);
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

void ObjectManager::CheckEnemyRemovalConditions(Enemy*& pEnemy)
{
	// CONDITIONS AND VARIABLES
	const bool hasCollidedWithKirby{ pEnemy->IsActive() && m_pKirby->CheckCollisionWith(pEnemy)};
	const bool outsideExtendedViewingArea
	{
		abs(pEnemy->GetShape().left - m_ViewLocation.x) > (m_ViewSize.x * 2 / 3)
		|| abs(pEnemy->GetShape().bottom - m_ViewLocation.y) > (m_ViewSize.y * 2 / 3)
	};
	const bool fellOutOfWorld{ pEnemy->GetShape().bottom < -50.f };
	const bool hasPassedKirby{ m_pKirby->GetDirection() > 0.f ? m_pKirby->GetShape().left > pEnemy->GetShape().left : m_pKirby->GetShape().left < pEnemy->GetShape().left };
	float direction{};

	// ACTUAL CODE
	
	if (m_pProjectileManager->ProjectileHasHit(pEnemy, Projectile::ActorType::enemy, direction) || fellOutOfWorld || outsideExtendedViewingArea)
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
		m_pKirby->BounceOffInDirection(bounceDirection);
		m_pKirby->DecrementHealth();
		pEnemy->Reset();
	}
}

void ObjectManager::UpdateEnemy(Enemy*& pEnemy, float elapsedSec)
{
	const float viewExtension{ 32.f };
	// const float reductionFactor{ m_ViewSize.x + 2 *  };
	// const bool insideXScreenBounds{ abs(pEnemy->GetShape().left - m_pKirby->GetShape().left) < (m_ViewSize.x * reductionFactor) };
	// const bool insideYScreenBounds{ abs(pEnemy->GetShape().bottom - m_pKirby->GetShape().bottom) < (m_ViewSize.y * reductionFactor) };
	const bool insideXScreenBounds{ abs(pEnemy->GetShape().left - m_ViewLocation.x) < ((m_ViewSize.x + viewExtension) / 2) };
	const bool insideYScreenBounds{ abs(pEnemy->GetShape().bottom - m_ViewLocation.y) < ((m_ViewSize.y + viewExtension) / 2) };

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

		CheckEnemyRemovalConditions(pEnemy);
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

void ObjectManager::UpdateItem(Item*& pItem, float elapsedSec)
{
	// If the item is gone, the game should no longer update the item
	if (pItem->IsRemoved())
	{
		return;
	}

	const bool hasCollidedWithKirby{ m_pKirby->CheckCollisionWith(pItem)};
	const bool outsideExtendedViewingArea
	{
		abs(pItem->GetShape().left - m_ViewLocation.x) > (m_ViewSize.x * 2 / 3)
		|| abs(pItem->GetShape().bottom - m_ViewLocation.y) > (m_ViewSize.y * 2 / 3)
	};
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

void ObjectManager::SupplyViewportDimensions(const Point2f& viewDimensions)
{
	m_ViewSize = viewDimensions;
}

std::vector<Enemy*>& ObjectManager::GetEnemyVector()
{
	return m_pEnemies;
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
		
		int firstCommaPos{ int(bufferString.find(',')) };
		int indexAfterFirstComma{ firstCommaPos + 1 };
		int secondCommaPos{ int(bufferString.find(',', indexAfterFirstComma)) };
		int indexAfterSecondComma{ secondCommaPos + 1 };
		int endPos{ int(bufferString.size())};
		
		std::string enemyName{ bufferString.substr(0, firstCommaPos) };
		float xCoordinate{ std::stof(bufferString.substr(indexAfterFirstComma, secondCommaPos)) };
		float yCoordinate{ std::stof(bufferString.substr(indexAfterSecondComma, endPos)) };
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
}
