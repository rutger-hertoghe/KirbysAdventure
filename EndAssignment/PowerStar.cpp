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
	m_ArbitraryTimer += elapsedSec;

	// Check timed events such as flickering out of existence and actual disappearing
	if (m_ArbitraryTimer > 10.f)
	{
		m_IsRemoved = true;
	}
	else if (m_ArbitraryTimer > 8.f &&  int(m_ArbitraryTimer * 40) % 2 == 0 )
	{
		m_NeedsToBeDrawn = !m_NeedsToBeDrawn;
	}
	else if (m_ArbitraryTimer > 5.f &&  int(m_ArbitraryTimer * 10) % 2 == 0 )
	{
		m_NeedsToBeDrawn = !m_NeedsToBeDrawn;
	}

	if (m_IsOnGround)
	{
		m_Velocity.y = m_BaseVelocity.y;
	}
	
	ChangeDirectionOnBump();
	
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
