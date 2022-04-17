#include "pch.h"
#include "LaserBall.h"
#include "Sprite.h"
#include <cmath>

LaserBall::LaserBall(const Point2f& location)
	: Enemy{location}
	, m_IsReboundingY(false)
	, m_IsInPlaceX(false)
	, m_IsInPlaceY(false)
	, m_IsLocked(false)
{
	SetBaseVelocity(0.f, 0.f);
	InitializeSprites();
	SetInitialSprite();
	SetDimsFromSprite();
	int maxCharges{ 4 }; // Will actually be one less so code below is ok
	int minCharges{ 1 };
	m_Charges = rand() % maxCharges + minCharges;
}

void LaserBall::Reset()
{
	Enemy::Reset();
	m_IsReboundingY = false;
	m_IsInPlaceX = false;
	m_IsInPlaceY = false;
	m_IsLocked = false;
	m_ArbitraryTimer = 0.f;
}

void LaserBall::InitializeSprites()
{
	m_pSprites.push_back(new Sprite{ 2, 0.15f, "laserball" });
	m_pSprites.push_back(new Sprite{ 2, 0.1f, "laserball_charge" });
	m_pSprites.push_back(new Sprite{ 2, 0.2f, "laserball_shot" });
}

void LaserBall::Update(float elapsedSec)
{
	UpdateSprite(elapsedSec);
	

	LockIfReady();
	UpdateTimerOrFlyAway(elapsedSec);
	FireIfNecessary();
	ChangePowerSprites();

	ApplyVelocities(elapsedSec, m_XDirection * m_Velocity.x, m_Velocity.y);
	
}

void LaserBall::DoChecksOnKirby(Kirby* pKirby)
{
	if (IsBeingInhaled()) return;

	const float velocityIncrease{ 2.f };
	Rectf kirbyShape{ pKirby->GetShape() };

	if (m_IsLocked == false)
	{
		MoveOverY(velocityIncrease, kirbyShape);
		MoveOverX(velocityIncrease, kirbyShape);
	}
}



void LaserBall::LockIfReady()
{
	if (m_IsInPlaceX && m_IsInPlaceY && !m_IsLocked)
	{
		m_IsLocked = true;
		m_pCurrentSprite = GetSpritePtr("laserball_charge");
	}
}

void LaserBall::UpdateTimerOrFlyAway(float elapsedSec)
{
	if (m_IsLocked && !m_IsBeingInhaled && m_pCurrentSprite->GetName() != "laserball")
	{
		m_Velocity.x = 0.f;
		m_Velocity.y = 0.f;
		m_ArbitraryTimer += elapsedSec;
	}
	else if (m_IsLocked && !m_IsBeingInhaled)
	{
		m_Velocity.y += 5.f;
	}
}

void LaserBall::FireIfNecessary()
{
	const float chargeTime{ 1.f };
	if (m_IsLocked && m_ArbitraryTimer > chargeTime && m_pCurrentSprite->GetName() == "laserball_charge")
	{
		m_pCurrentSprite = GetSpritePtr("laserball_shot");
	}
}

void LaserBall::ChangePowerSprites()
{
	if (m_pCurrentSprite->GetName() == "laserball_shot" && m_HasLooped)
	{
		m_ArbitraryTimer = 0.f;
		if (m_Charges > 0)
		{
			--m_Charges;
		}
		else if (m_pCurrentSprite->GetName() != "laserball")
		{
			m_pCurrentSprite = GetSpritePtr("laserball");
		}
	}
}

void LaserBall::MoveOverY(float velocityIncrease, const Rectf& kirbyShape)
{
	const float targetY{ kirbyShape.bottom };
	const float distToKirbyY{ abs(m_Shape.bottom - kirbyShape.bottom) };
	float distanceTolerance{ 1.f };

	if (m_Shape.bottom > kirbyShape.bottom && m_IsReboundingY == false)
	{
		m_Velocity.y -= velocityIncrease;
	}
	else if (distToKirbyY < distanceTolerance && m_IsReboundingY)
	{
		m_Velocity.y = 0.f;
	}
	else if (distToKirbyY > distanceTolerance && m_IsReboundingY)
	{
		const float maxVelocityY{ targetY - m_Shape.bottom };
		if (m_Velocity.y > maxVelocityY)
		{
			m_Velocity.y = maxVelocityY;
		}
		else
		{
			const float magnification{ 20.f };
			m_Velocity.y += velocityIncrease * magnification;
		}
	}
	else if (m_Shape.bottom < kirbyShape.bottom)
	{
		m_IsReboundingY = true;
	}
	
	distanceTolerance = 5.f;
	const float velocityTolerance{ 5 * distanceTolerance };
	if (abs(distToKirbyY) < distanceTolerance && abs(m_Velocity.y) < velocityTolerance)
	{
		m_IsInPlaceY = true;
	}
}

void LaserBall::MoveOverX(float velocityIncrease, const Rectf& kirbyShape)
{
	const bool leftOfKirby{ m_Shape.left < kirbyShape.left };
	const float distanceTolerance{ 10.f };
	const float targetDistanceX{ 80.f };
	const float targetX{ leftOfKirby ? kirbyShape.left - targetDistanceX : kirbyShape.left + kirbyShape.width + targetDistanceX };
	const float distToKirbyX{ abs(m_Shape.left - kirbyShape.left) };

	m_XDirection = leftOfKirby ? 1.f : -1.f;

	if (abs(distToKirbyX - targetDistanceX) > distanceTolerance)
	{
		const float magnification{ 5.f };
		m_Velocity.x = m_XDirection * magnification * (targetX - m_Shape.left);
		m_IsInPlaceX = false;
	}
	else
	{
		m_IsInPlaceX = true;
	}
}