#include "pch.h"
#include "Fireball.h"
#include "utils.h"
#include "Level.h"
#include "LevelManager.h"
#include "ObjectManager.h"

Fireball::Fireball(Actor* pOwner, const Rectf& fireballRect, const Vector2f& directionVector)
	: Projectile{pOwner, pOwner->GetLevelManager(), ProjectileType::fireball, fireballRect, Vector2f{directionVector.x * 200, directionVector.y} }
	, m_MaxDistance{ 32.0f }
	, m_FlownDistance{0.f}
{
	m_XDirection = directionVector.x;
	m_Velocity.y *= m_Shape.height;
	m_IsPersistent = true;
}

void Fireball::Update(float elapsedSec)
{
	UpdateSprite(elapsedSec);
	ApplyVelocities(elapsedSec);
	m_FlownDistance += elapsedSec * m_Velocity.x;
	if (abs(m_FlownDistance) > m_MaxDistance)
	{
		m_IsReadyToDestroy = true;
	}
	DestroyOnCollision();
}



