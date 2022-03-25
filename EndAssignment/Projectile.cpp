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

Projectile::~Projectile()
{

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

void Projectile::SetSprite(Sprite* spritePtr)
{
	m_pCurrentSprite = spritePtr;
}