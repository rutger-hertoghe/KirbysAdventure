#include "pch.h"
#include "StonePower.h"

StonePower::StonePower()
	: PowerUp(PowerUpType::stone, true, true, true)
{
}

void StonePower::OnKeyDownEvent()
{
	m_IsActive = !m_IsActive;
}

void StonePower::ContinuousKeyEvent()
{
}

void StonePower::OnKeyUpEvent()
{
}

void StonePower::Update(float elapsedSec)
{
}

bool StonePower::IsActive() const
{
	return m_IsActive;
}

std::string StonePower::GetPowerSuffix() const
{
	return "stone_";
}
