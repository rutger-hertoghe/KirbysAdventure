#include "pch.h"
#include "Sparky.h"
#include "Sprite.h"
#include "SparkPower.h"

Sparky::Sparky(const Point2f& location)
	: Enemy{location}
	, m_IsPowerUsable{ false }
	, m_IsInert{ false }
{
	InitializeSprites();
	SetInitialSprite();
	SetDimsFromSprite();
	SetBaseVelocity(Point2f{ 30.f, 150.f });
}

void Sparky::Update(float elapsedSec)
{
	if (GetPowerUp()->IsActive() == false)
	{
		HandleJumping();
	}

	ApplyVelocities(elapsedSec, m_XDirection * m_Velocity.x, m_Velocity.y);
	if (m_IsBeingInhaled) return; // If enemy is being inhaled, code below can not be executed

	const float powerDuration{ 2.f };
	const float inertiaDuration{ 3.f };
	const float maxInertiaTime{ powerDuration + inertiaDuration };
	if (HasPowerUp())
	{
		if (m_IsPowerUsable && m_IsInert == false)
		{
			m_IsInert = true;
			m_IsPowerUsable = false;
			m_pCurrentSprite = GetSpritePtr("sparky_ability");
			m_Velocity.x = 0.f;
			m_pPowerUp->OnKeyDownEvent(m_Shape, m_XDirection);
		}
		else if (m_ArbitraryTimer < powerDuration && m_IsInert)
		{
			m_pPowerUp->ContinuousKeyEvent(m_Shape, m_XDirection);
			m_pPowerUp->Update(elapsedSec);
		}
		else if (powerDuration < m_ArbitraryTimer && m_pPowerUp->IsActive())
		{
			m_pCurrentSprite = GetSpritePtr("sparky");
			m_pPowerUp->OnKeyUpEvent(m_Shape, m_XDirection);
		}
		else if (maxInertiaTime < m_ArbitraryTimer && m_IsInert)
		{
			m_ArbitraryTimer = 0.f;
			m_IsInert = false;
		}
	}

	if (m_IsInert == true) // Counter should only run when using power or during inertia period
	{
		m_ArbitraryTimer += elapsedSec;
	}

	UpdateSprite(elapsedSec);
	ApplyGravity(elapsedSec);
	HandleLevelCollisions();
	SetIsOnGround();
}

void Sparky::InitializePowerUp()
{
	SetPowerUp(new SparkPower{ m_pProjectileManager });
}

void Sparky::DoChecksOnKirby(Kirby* pKirby)
{
	const bool isNotApproachingKirby{(pKirby->GetShape().left - m_Shape.left) * m_XDirection < 0.f};
	if (m_Velocity.x == 0.f && isNotApproachingKirby)
	{
		m_XDirection = -m_XDirection;
	}

	const float sqDistToKirby{ GetSquaredDistanceToActor(pKirby) };
	const float sqDistanceTreshold{ powf(64.f, 2) };
	if (sqDistToKirby < sqDistanceTreshold)
	{
		m_IsPowerUsable = true;
	}
}

void Sparky::InitializeSprites()
{
	m_pSprites.push_back(new Sprite{ 2, 0.2f, "sparky" });
	m_pSprites.push_back(new Sprite{ 2, 0.1f, "sparky_ability" });
	m_pSprites.push_back(new Sprite{ 1, 0.f, "sparky_jump" });
}

void Sparky::HandleJumping()
{
	if (m_IsOnGround && m_pCurrentSprite->GetName() == "sparky_jump")
	{
		m_pCurrentSprite = GetSpritePtr("sparky");
		m_Velocity.x = 0.f;
	}
	else if (m_pCurrentSprite->GetName() == "sparky" && m_HasLooped && m_IsOnGround)
	{
		m_pCurrentSprite = GetSpritePtr("sparky_jump");
		m_Velocity.x = m_BaseVelocity.x;
		m_Velocity.y = m_BaseVelocity.y;
	}
}
