#include "pch.h"
#include "Star.h"

Star::Star(Rectf starRect, float xDirection)
	: Projectile{ ProjectileType::star, starRect, Vector2f{xDirection * 220.f, 0.f} }
	, m_LifeTime{0.f}
	, m_MaxTime{2.0f}
{
}

Star::~Star()
{
}

void Star::Update(float elapsedSec)
{
	UpdateSprite(elapsedSec);
	m_Shape.left += m_Velocity.x * elapsedSec;
	m_LifeTime += elapsedSec;
	if (m_LifeTime > m_MaxTime)
	{
		m_IsReadyToDestroy = true;
	}
}
