#include "pch.h"
#include "FirePower.h"
#include "ObjectManager.h"
#include "ProjectileManager.h"
#include "utils.h"
#include "Fireball.h"
#include "Vector2f.h"

FirePower::FirePower(Actor* pOwner)
	: PowerUp{PowerUpType::fire, pOwner, false, true, false}
	, m_TimeBetweenSpawns{0.08f}
	, m_TimeSinceSpawn{0.f}
	, m_CanSpawn{true}
	, m_YDirection{0}
	, m_AngleDifference{1}
{
}


void FirePower::OnKeyDownEvent()
{
	m_IsActive = true;
}

void FirePower::ContinuousKeyEvent()
{
	Rectf ownerShape{ m_pOwner->GetShape() };
	float ownerDirection{ m_pOwner->GetDirection() };

	if (m_CanSpawn == true)
	{
		float pixelReduction{ -2.f }; // To correct whether kirby's fireballs go through wall
		Rectf spawnLocation{ 0.f, ownerShape.bottom, ownerShape.width, ownerShape.height };
		spawnLocation.left = ownerShape.left + (ownerDirection > 0.f ? ownerShape.width + pixelReduction: -ownerShape.width - pixelReduction);

		Vector2f directionVector{ ownerDirection, float(m_YDirection * 3) };

		if (m_pProjectileManager)
		{
			m_pProjectileManager->Add(new Fireball{ m_pOwner, spawnLocation, directionVector });
		}
		else std::cout << "PROJECTILE MANAGER NOT FOUND\n";
		m_CanSpawn = false;

		SoundFXManager::StopAll();
	}
	SoundFXManager::Play("fire");
}

void FirePower::OnKeyUpEvent()
{
	m_IsActive = false;
}

void FirePower::Update(float elapsedSec)
{
	m_TimeSinceSpawn += elapsedSec;
	if (m_TimeSinceSpawn > m_TimeBetweenSpawns)
	{
		m_TimeSinceSpawn = 0.f;
		m_CanSpawn = true;
		ChangeFireAngle();
	}
}

bool FirePower::IsActive() const
{
	return m_IsActive;
}

std::string FirePower::GetPowerSuffix() const
{
	return "fire_";
}

void FirePower::ChangeFireAngle()
{
	if (m_YDirection > 0)
	{
		m_AngleDifference = -1;
	}
	else if (m_YDirection < 0)
	{
		m_AngleDifference = 1;
	}
	m_YDirection += m_AngleDifference;
}
