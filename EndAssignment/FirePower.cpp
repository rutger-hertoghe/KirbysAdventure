#include "pch.h"
#include "FirePower.h"
#include "ObjectManager.h"
#include "ProjectileManager.h"
#include "utils.h"
#include "Fireball.h"
#include "Vector2f.h"

FirePower::FirePower()
	: PowerUp{PowerUpType::fire, false, true, false}
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
	if (m_CanSpawn == true)
	{
		float pixelReduction{ -2.f }; // To correct whether kirby's fireballs go through wall
		Rectf ownerShape{ m_pOwner->GetShape() };
		float ownerDirection{ m_pOwner->GetDirection() };
		Rectf spawnLocation{ 0.f, ownerShape.bottom, ownerShape.width, ownerShape.height};
		spawnLocation.left = ownerShape.left + (ownerDirection > 0.f ? ownerShape.width + pixelReduction : -ownerShape.width - pixelReduction);

		Vector2f directionVector{ ownerDirection, float(m_YDirection * 3) };

		ProjectileManager* pProjectileMngr = ProjectileManager::GetProjectileMngr();
		if (pProjectileMngr)
		{
			pProjectileMngr->Add(new Fireball{ m_Owner, spawnLocation, directionVector });
		}
		else std::cout << "PROJECTILE MANAGER NOT FOUND\n";
		m_CanSpawn = false;
	}
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
