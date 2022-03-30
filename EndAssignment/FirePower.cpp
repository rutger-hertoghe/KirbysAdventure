#include "pch.h"
#include "FirePower.h"
#include "ProjectileManager.h"
#include "utils.h"
#include "Fireball.h"
#include <iostream>
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

void FirePower::OnKeyDownEvent(const Rectf& shape, float xDirection)
{
	m_IsActive = true;
}

void FirePower::ContinuousKeyEvent(const Rectf& shape, float xDirection)
{
	if (m_CanSpawn == true)
	{
		Rectf spawnLocation{ 0.f, shape.bottom, shape.width, shape.height };
		spawnLocation.left = shape.left + (xDirection > 0.f ? shape.width : -shape.width);
		Vector2f directionVector{ xDirection, float(m_YDirection * 3) };
		if (m_pProjectileManager) m_pProjectileManager->Add(new Fireball{ spawnLocation, directionVector });
		else std::cout << "PROJECTILE MANAGER NOT FOUND\n";
		m_CanSpawn = false;
	}
}

void FirePower::OnKeyUpEvent(const Rectf& shape, float xDirection)
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
