#include "pch.h"
#include "StonePower.h"

StonePower::StonePower()
	: PowerUp(PowerUpType::stone, nullptr, true, true, true)
{
	
}

void StonePower::OnKeyDownEvent(const Rectf& shape, float xDirection)
{
	m_IsActive = !m_IsActive;
}

void StonePower::ContinuousKeyEvent(const Rectf& shape, float xDirection)
{
}

void StonePower::OnKeyUpEvent(const Rectf& shape, float xDirection)
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
