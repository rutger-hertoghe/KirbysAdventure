#include "pch.h"
#include "Projectile.h"
#include "Sprite.h"
#include "ObjectManager.h"
#include "Level.h"
#include "LevelManager.h"
#include "Kirby.h"

Projectile::Projectile(Actor* pOwner, ProjectileType type, const Rectf& projectileRect, const Vector2f& velocity)
	: m_Type{type}
	, m_IsReadyToDestroy{false}
	, m_Velocity{velocity}
	, m_IsPersistent{false}
	, m_pOwner{ pOwner }
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

Actor* Projectile::GetOwner()
{
	return m_pOwner;
}

void Projectile::ApplyVelocities(float elapsedSec)
{
	m_Shape.left += elapsedSec * m_Velocity.x;
	m_Shape.bottom += elapsedSec * m_Velocity.y;
}

void Projectile::DestroyOnCollision()
{
	if (typeid(m_pOwner) == typeid(Kirby) && LevelManager::GetCurrentLevel()->IsAgainstWall(m_Shape, m_XDirection))
	{
		m_IsReadyToDestroy = true;
		ObjectManager::GetObjectMngr()->AddRemovalFX(Point2f{ m_Shape.left, m_Shape.bottom }, RemovalFX::FXType::item);
	}
}

void Projectile::SetSprite(Sprite* spritePtr)
{
	m_pCurrentSprite = spritePtr;
}