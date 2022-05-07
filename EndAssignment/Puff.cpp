#include "pch.h"
#include "Puff.h"
#include "Sprite.h"

Puff::Puff(Actor* pOwner, Rectf puffRect, float xDirection)
	: Projectile{ pOwner,  ProjectileType::puff, puffRect, Vector2f{0.f, 0.f} }
	, m_MaxDistance{ 40.f }
	, m_FlownDistance{ 0.0f }
{
	m_XDirection = xDirection;
	m_IsPersistent = false;
}

void Puff::Update(float elapsedSec)
{
	const float velocityScalar{ 5.f };
	m_Velocity.x = m_XDirection * velocityScalar * (m_MaxDistance - m_FlownDistance);
	m_FlownDistance += abs(m_Velocity.x) * elapsedSec;
	UpdateSprite(elapsedSec);
	ApplyVelocities(elapsedSec);
	const float deletionTreshold{ 5.f }; // Range from m_MaxDistance in which Puff can be flagged for removal
	if (m_MaxDistance - m_FlownDistance < deletionTreshold)
	{
		m_IsReadyToDestroy = true;
	}
	DestroyOnCollision();
}
