#include "pch.h"
#include "FirePower.h"
#include "utils.h"
#include <iostream>

FirePower::FirePower()
	: PowerUp{PowerUpType::fire, false, true, false}
	, m_DamageZone{}
{
}

FirePower::~FirePower()
{
}

void FirePower::OnKeyDownEvent(Rectf shape, float xDirection)
{
	m_IsActive = true;
	std::cout << "Fire powerup active!\n";
}

void FirePower::ContinuousKeyEvent(Rectf shape, float xDirection)
{
	m_DamageZone = Rectf{ shape.left + shape.width, shape.bottom, shape.width, shape.height };
}

void FirePower::OnKeyUpEvent(Rectf shape, float xDirection)
{
	m_IsActive = false;
	std::cout << "Fire power up no longer active!\n";
}

void FirePower::Draw() const
{
	if (m_IsActive)
	{
		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
		utils::FillRect(m_DamageZone);
	}
}

bool FirePower::IsActive() const
{
	return m_IsActive;
}

bool FirePower::IsOneShot() const
{
	return false;
}

std::string FirePower::GetPowerSuffix() const
{
	return "fire_";
}
