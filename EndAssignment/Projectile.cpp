#include "pch.h"
#include "Projectile.h"
#include "Sprite.h"

Projectile::Projectile(ProjectileType type, const Rectf& projectileRect, const Vector2f& velocity)
	: m_Type{type}
	, m_IsReadyToDestroy{false}
	, m_Velocity{velocity}
	, m_IsPersistent{false}
{
	m_Shape = projectileRect;
}

bool Projectile::IsReadyToDestroy() const
{
	return m_IsReadyToDestroy;
}

bool Projectile::IsPersistent() const
{
	return m_IsPersistent;
}

void Projectile::SetReadyToDestroy()
{
	m_IsReadyToDestroy = true;
}

int Projectile::GetTypeInt() const
{
	return int(m_Type);
}

void Projectile::ApplyVelocities(float elapsedSec)
{
	m_Shape.left += elapsedSec * m_Velocity.x;
	m_Shape.bottom += elapsedSec * m_Velocity.y;
}

void Projectile::SetSprite(Sprite* spritePtr)
{
	m_pCurrentSprite = spritePtr;
}