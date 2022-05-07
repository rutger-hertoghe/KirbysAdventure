#include "pch.h"
#include "Star.h"

Star::Star(Actor* pOwner, Rectf starRect, float xDirection)
	: Projectile{pOwner, ProjectileType::star, starRect, Vector2f{xDirection * 220.f, 0.f} }
{
}

void Star::Update(float elapsedSec)
{
	UpdateSprite(elapsedSec);
	m_Shape.left += m_Velocity.x * elapsedSec;
	DestroyOnCollision();
}
