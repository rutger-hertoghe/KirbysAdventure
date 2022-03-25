#include "pch.h"
#include "PowerStar.h"
#include "Sprite.h"

PowerStar::PowerStar(const Point2f& location)
{
	m_Shape.left = location.x;
	m_Shape.bottom = location.y;
	m_pCurrentSprite = new Sprite{ 4, 0.5f, "resources/sprites/star.png" };
	m_Shape.width = m_pCurrentSprite->GetFrameDimensions().x;
	m_Shape.height = m_pCurrentSprite->GetFrameDimensions().y;
	SetBaseVelocity( 50.f, 50.f);
}

PowerStar::~PowerStar()
{
	delete m_pCurrentSprite;
	m_pCurrentSprite = nullptr;
}

void PowerStar::Update(float elapsedSec)
{
	if (m_IsOnGround)
	{
		m_Velocity.y = -m_Gravity / 2;
	}

	SetIsOnGround();
	UpdateSprite(elapsedSec);
	ApplyVelocities(elapsedSec, m_Velocity.x, m_Velocity.y);
	ApplyGravity(elapsedSec);
	HandleCollisions();
}
