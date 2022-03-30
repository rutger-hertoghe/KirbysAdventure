#include "pch.h"
#include "EnemyManager.h"
#include "ProjectileManager.h"
#include "Enemy.h"
#include "WaddleDee.h"
#include "Level.h"

EnemyManager::EnemyManager()
	: m_pKirby{ nullptr }
	, m_pProjectileManager{ nullptr }
{
	Initialize();
}

EnemyManager::~EnemyManager()
{
	for (Enemy* pEnemy : m_pEnemies)
	{
		delete pEnemy;
	}
	m_pEnemies.clear();
}

void EnemyManager::Initialize()
{
	m_pEnemies.push_back(new WaddleDee{ Point2f{996.f, 100.f} });
}

void EnemyManager::Draw() const
{
	for (Enemy* pEnemy : m_pEnemies)
	if (pEnemy->IsActive())
	{
		pEnemy->Draw();
	}
}

void EnemyManager::Update(float elapsedSec)
{
	for (Enemy*& pEnemy : m_pEnemies)
	{
		if (pEnemy) UpdateEnemy(pEnemy, elapsedSec);
	}
}

void EnemyManager::LinkProjectileManager(ProjectileManager* managerPtr)
{
	m_pProjectileManager = managerPtr;
}

void EnemyManager::LinkKirby(Kirby* kirbyPtr)
{
	m_pKirby = kirbyPtr;
}

void EnemyManager::LinkLevel(Level* levelPtr)
{
	for (Enemy* pEnemy : m_pEnemies)
	{
		pEnemy->SetCurrentLevel(levelPtr);
	}
}

void EnemyManager::CheckEnemyRemovalConditions(Enemy*& pEnemy)
{
	if (m_pProjectileManager->ProjectileHasHit(pEnemy))
	{
		pEnemy->Reset();
	}
	else if (pEnemy->CheckCollision(m_pKirby->GetRect()))
	{
		if (m_pKirby->IsInhaling())
		{
			m_pKirby->SetBloated();
			if (pEnemy->GetPowerUp())
			{
				pEnemy->TransferPowerUp(m_pKirby);
			}
		}
		else
		{
			m_pKirby->DecrementHealth();
		}
		pEnemy->Reset();
	}
	else if (pEnemy->GetRect().bottom < -50.f)
	{
		pEnemy->Reset();
	}
}

void EnemyManager::UpdateEnemy(Enemy*& pEnemy, float elapsedSec)
{
	const bool isOffScreenRight{};
	const bool isOffScreenLeft{};

	if (pEnemy->IsActive())
	{
		pEnemy->Update(elapsedSec);

		if (pEnemy->IsInhalable())
		{
			pEnemy->ToggleBeingInhaled(m_pKirby->GetInhalationZone());
		}

		if (pEnemy->IsBeingInhaled())
		{
			pEnemy->SetInhalationVelocities(m_pKirby->GetRect());
		}

		CheckEnemyRemovalConditions(pEnemy);
	}
	else if (pEnemy->HasBeenOffScreen()
		&& abs(pEnemy->GetRect().left - m_pKirby->GetRect().left) < viewSize.x * 2 / 3
		&& abs(pEnemy->GetRect().bottom - m_pKirby->GetRect().bottom) < viewSize.y * 2 / 3)
	{
		pEnemy->SetActivity(true);
	}
	else if (abs(pEnemy->GetRect().left - m_pKirby->GetRect().left) > viewSize.x * 2 / 3
		|| abs(pEnemy->GetRect().bottom - m_pKirby->GetRect().bottom) > viewSize.y * 2 / 3)
	{
		float xDirection{ pEnemy->GetRect().left - m_pKirby->GetRect().left > 0.f ? -1.f : 1.f };
		pEnemy->SetOffScreen(true, xDirection);
	}
}

void EnemyManager::SupplyViewportDimensions(const Point2f& viewDimensions)
{
	viewSize = viewDimensions;
}
