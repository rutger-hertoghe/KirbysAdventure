#include "pch.h"
#include "PowerStar.h"
#include "Sprite.h"

PowerStar::PowerStar(const Point2f& location)
	: Item{false, false, false, false, true, false}
{
	SetLocation(location);
	InitializeSprites();
	SetInitialSprite();
	SetDimsFromSprite();
	SetBaseVelocity( 80.f, 200.f);
}

PowerStar::~PowerStar()
{
}

void PowerStar::Update(float elapsedSec)
{
	m_ArbitraryTimer += elapsedSec;

	// TODO: Remove hardcoded values!!
	// Check timed events such as flickering out of existence and actual disappearing
	if (m_ArbitraryTimer > 5.f)
	{
		m_IsRemoved = true;
	}
	else if (m_ArbitraryTimer > 4.f &&  int(m_ArbitraryTimer * 40) % 2 == 0 )
	{
		m_NeedsToBeDrawn = !m_NeedsToBeDrawn;
	}
	else if (m_ArbitraryTimer > 3.f &&  int(m_ArbitraryTimer * 10) % 2 == 0 )
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

void PowerStar::InitializeSprites()
{
	m_pSprites.push_back(new Sprite{ 4, 0.5f, "star" });
}