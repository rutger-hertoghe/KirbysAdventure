#include "pch.h"
#include "Rocky.h"
#include "Sprite.h"
#include "StonePower.h"
#include "Camera.h"

Rocky::Rocky(const Point2f& location)
	: Enemy{location}
	, m_MaxStateTime{5.f}
	, m_IsAbilityActive{false}
{
	InitializeSprites();
	SetInitialSprite();
	SetDimsFromSprite();
	SetBaseVelocity(30.f, 0.f);

	m_XDirection = 1.f;

	m_ArbitraryTimer = 0.f;
}

void Rocky::Update(float elapsedSec)
{
	if (IsBeingInhaled())
	{
		ApplyVelocities(elapsedSec, m_XDirection * m_Velocity.x, m_Velocity.y);
		return;
	}

	m_ArbitraryTimer += elapsedSec;
	ChangeState();

	ApplyVelocities(elapsedSec, m_XDirection * m_Velocity.x, m_Velocity.y);
	UpdateSprite(elapsedSec);
	ApplyGravity(elapsedSec);
	HandleLevelCollisions();
	SetIsOnGround();
	ChangeDirectionOnBump();
}

void Rocky::Reset()
{
	Enemy::Reset();
	m_ArbitraryTimer = 0;
	m_IsAbilityActive = false;
}

void Rocky::InitializeSprites()
{
	m_pSprites.push_back(new Sprite{2, 0.4f, "rocky" });
	m_pSprites.push_back(new Sprite{ 1, 0.f, "rocky_ability" });
}

void Rocky::InitializePowerUp()
{
	SetPowerUp(new StonePower{});
}

void Rocky::ChangeState()
{
	if (m_ArbitraryTimer > m_MaxStateTime)
	{
		m_ArbitraryTimer = 0.f;
		m_IsAbilityActive = !m_IsAbilityActive;
		if (m_IsAbilityActive)
		{
			m_pCurrentSprite = GetSpritePtr("rocky_ability");
			m_Velocity.x = 0;
			Camera::SetShake();
		}
		else
		{
			m_pCurrentSprite = GetSpritePtr("rocky");
			m_Velocity.x = m_BaseVelocity.x;
		}
	}
}
