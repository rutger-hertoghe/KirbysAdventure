#include "pch.h"
#include "StonePower.h"
#include "Camera.h"
#include "LevelManager.h"
#include "Level.h"

StonePower::StonePower(Actor* pOwner)
	: PowerUp(PowerUpType::stone, pOwner, true, true, true)
	, m_IsActorOnGround{true}
	, m_WasActorAlreadyOnGround{true}
{
}

void StonePower::OnKeyDownEvent()
{
	m_IsActive = !m_IsActive;
	if (m_pOwner->IsOnGround())
	{
		const float bumpVelocity{ 100.f };
		m_pOwner->AddVelocity(0.f, bumpVelocity);
	}
}

void StonePower::ContinuousKeyEvent()
{
}

void StonePower::OnKeyUpEvent()
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
	}
	m_WasActorAlreadyOnGround = m_IsActorOnGround;
	m_IsActorOnGround = m_pOwner->IsOnGround();
}

bool StonePower::IsActive() const
{
	return m_IsActive;
}

std::string StonePower::GetPowerSuffix() const
{
	return "stone_";
}
