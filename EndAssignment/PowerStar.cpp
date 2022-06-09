#include "pch.h"
#include "PowerStar.h"
#include "Sprite.h"

PowerStar::PowerStar(const Point2f& location, LevelManager* pLevelManager, ProjectileManager* pProjectileManager)
	: Item{pLevelManager, pProjectileManager, false, false, false, false, true, false}
{
	SetLocation(location);
	InitializeSprites();
	SetInitialSprite();
	SetDimsFromSprite();
	SetBaseVelocity( 80.f, 200.f);
}

void PowerStar::Update(float elapsedSec)
{
	m_ArbitraryTimer += elapsedSec;

	UpdateStarFlicker();
	
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

void PowerStar::UpdateStarFlicker()
{
	const float flickerTime{ 3.f };
	const float fasterFlickerTime{ 4.f };
	const float dissappearTime{ 5.f };

	const int fastInstabilityCoefficient{ 40 };
	const int slowInstabilityCoefficient{ 10 };

	// Check timed events such as flickering out of existence and actual disappearing
	if (m_ArbitraryTimer > dissappearTime)
	{
		m_IsRemoved = true;
	}
	else if (m_ArbitraryTimer > fasterFlickerTime && int(m_ArbitraryTimer * fastInstabilityCoefficient) % 2 == 0)
	{
		m_NeedsToBeDrawn = !m_NeedsToBeDrawn;
	}
	else if (m_ArbitraryTimer > flickerTime && int(m_ArbitraryTimer * slowInstabilityCoefficient) % 2 == 0)
	{
		m_NeedsToBeDrawn = !m_NeedsToBeDrawn;
	}

	if (m_IsOnGround)
	{
		m_Velocity.y = m_BaseVelocity.y;
	}
}
