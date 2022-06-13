#include "pch.h"
#include "SparkPower.h"
#include "ObjectManager.h"
#include "ProjectileManager.h"
#include "Spark.h"
#include <iostream>

SparkPower::SparkPower(Actor* pOwner)
	: PowerUp{PowerUpType::spark, pOwner, true, true, false}
	, m_CanSpawn{ false }
	, m_TimeBetweenSpawns{ 0.05f }
	, m_TimeSinceSpawn{ 0.0f }
{
}

void SparkPower::OnKeyDownEvent()
{
	m_IsActive = true;
}

void SparkPower::ContinuousKeyEvent()
{
	Rectf ownerShape{ m_pOwner->GetShape() };

	const int directions{ 8 };
	const int spawnDirection{ rand() % directions };
	const float spawnAngle{ float(M_PI) / 4 * spawnDirection };
	const float cosAngle{ cosf(spawnAngle) }, sinAngle{ sinf(spawnAngle) };
	const float spawnOffset{ 8.f }, velocity{ 100.f };
	const Rectf spawnRect{ ownerShape.left + (spawnOffset * cosAngle), ownerShape.bottom + (spawnOffset * sinAngle), ownerShape.width, ownerShape.height };
	const Vector2f spawnVelocities{ velocity * cosf(spawnAngle), velocity * sinf(spawnAngle) };
	if (m_CanSpawn)
	{
		m_pProjectileManager->Add(new Spark{m_pOwner, spawnRect, spawnVelocities});
		SoundFXManager::Play("shock");
		m_CanSpawn = false;
	}
}

void SparkPower::OnKeyUpEvent()
{
	m_IsActive = false;
}

bool SparkPower::IsActive() const
{
	return m_IsActive;
}

std::string SparkPower::GetPowerSuffix() const
{
	return "spark_";
}

void SparkPower::Update(float elapsedSec)
{
	m_TimeSinceSpawn += elapsedSec;
	if (m_TimeSinceSpawn > m_TimeBetweenSpawns)
	{
		m_TimeSinceSpawn = 0.f;
		m_CanSpawn = true;
	}
}
