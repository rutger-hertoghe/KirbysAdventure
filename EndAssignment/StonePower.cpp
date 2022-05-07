#include "pch.h"
#include "StonePower.h"
#include "Camera.h"
#include "LevelManager.h"
#include "Level.h"

StonePower::StonePower()
	: PowerUp(PowerUpType::stone, true, true, true)
	, m_IsActorOnGround(true)
	, m_WasActorAlreadyOnGround(true)
{
}

void StonePower::OnKeyDownEvent(const Rectf& shape, float xDirection)
{
	m_IsActive = !m_IsActive;
}

void StonePower::ContinuousKeyEvent(const Rectf& shape, float xDirection)
{
	m_ActorShape = shape;
}

void StonePower::OnKeyUpEvent(const Rectf& shape, float xDirection)
{
}

void StonePower::Update(float elapsedSec)
{
	if (m_IsActive)
	{
		if (m_IsActorOnGround && !m_WasActorAlreadyOnGround)
		{
			Camera::SetShake();
		}
		m_WasActorAlreadyOnGround = m_IsActorOnGround;
		m_IsActorOnGround = LevelManager::GetCurrentLevel()->IsOnGround(m_ActorShape);
	}
}

bool StonePower::IsActive() const
{
	return m_IsActive;
}

std::string StonePower::GetPowerSuffix() const
{
	return "stone_";
}
