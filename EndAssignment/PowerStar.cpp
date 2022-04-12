#include "pch.h"
#include "PowerStar.h"
#include "Sprite.h"

PowerStar::PowerStar(const Point2f& location)
	: Item{false, true, false}
{
	SetLocation(location);
	m_pCurrentSprite = new Sprite{ 4, 0.5f, "star" };
	m_Shape.width = m_pCurrentSprite->GetFrameDimensions().x;
	m_Shape.height = m_pCurrentSprite->GetFrameDimensions().y;
	SetBaseVelocity( 80.f, 200.f);
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
		m_Velocity.y = m_BaseVelocity.y;
	}
	if (m_Velocity.x == 0.f)
	{
		m_Velocity.x = m_BaseVelocity.x;
		m_XDirection *= -1.f;
	}

	SetIsOnGround();
	UpdateSprite(elapsedSec);
	ApplyVelocities(elapsedSec, m_XDirection * m_Velocity.x, m_Velocity.y);
	ApplyGravity(elapsedSec);
	HandleLevelCollisions();
}

void PowerStar::SetDirection(float directionValue)
{
	m_XDirection = directionValue;
}
