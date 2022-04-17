#include "pch.h"
#include "ThrowingStar.h"
#include "Sprite.h"

ThrowingStar::ThrowingStar(Rectf starRect, float xDirection)
	: Projectile{ Projectile::ActorType::enemy, ProjectileType::throwingStar, starRect, Vector2f{xDirection * 180.f, 120.f} }
	, m_MaxExistenceTime{5.f}
	, m_ExistenceTime{0.f}
{
	const float size{ 8.f };
	m_Shape.width = size;
	m_Shape.height = size;
}

void ThrowingStar::Update(float elapsedSec)
{
	const float gravity{ -200.f };
	m_Velocity.y += gravity * elapsedSec;
	m_Shape.left += elapsedSec * m_Velocity.x;
	m_Shape.bottom += elapsedSec * m_Velocity.y;

	m_ExistenceTime += elapsedSec;
	if (m_ExistenceTime > m_MaxExistenceTime)
	{
		SetReadyToDestroy();
	}
}
