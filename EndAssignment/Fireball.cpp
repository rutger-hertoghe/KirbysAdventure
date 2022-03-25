#include "pch.h"
#include "Fireball.h"
#include "utils.h"

Fireball::Fireball(Rectf fireballRect, Vector2f directionVector)
	: Projectile{ ProjectileType::fireball, fireballRect, Vector2f{directionVector.x * 200, directionVector.y} }
	, m_MaxDistance{ 32.0f }
	, m_FlownDistance{0.f}
{
	m_XDirection = directionVector.x;
	m_Velocity.y *= m_Shape.height;
	m_IsPersistent = true;
}

Fireball::~Fireball()
{
}

void Fireball::Update(float elapsedSec)
{
	UpdateSprite(elapsedSec);
	m_Shape.left += elapsedSec * m_Velocity.x;
	m_Shape.bottom += elapsedSec * m_Velocity.y;
	m_FlownDistance += elapsedSec * m_Velocity.x;
	if (abs(m_FlownDistance) > m_MaxDistance)
	{
		m_IsReadyToDestroy = true;
	}
}



