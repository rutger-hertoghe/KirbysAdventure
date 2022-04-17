#include "pch.h"
#include "Bounder.h"
#include "Sprite.h"
#include "ProjectileManager.h"
#include "ThrowingStar.h"
#include "LevelManager.h"

Bounder::Bounder(const Point2f& location)
	: Enemy{location}
	, m_IsOnWall{false}
	, m_IsFirstUpdateCall{true}
{
	InitializeSprites();
	SetInitialSprite();
	SetDimsFromSprite();
	SetBaseVelocity(50.f, 50.f);
}

void Bounder::Update(float elapsedSec)
{
	if (m_IsFirstUpdateCall)
	{
		SetStationary();
		CheckBothWalls();
		m_IsFirstUpdateCall = false;
	}

	ApplyVelocities(elapsedSec, m_XDirection * m_Velocity.x, m_Velocity.y);
	if (m_IsBeingInhaled) return; // If enemy is being inhaled, code below can not be executed

	GenerateNumToCheckOdds();

	const bool wasOnWall{ m_IsOnWall };
	m_IsOnWall = IsOnWall();

	const bool isOnLedge{ m_IsOnWall == false && wasOnWall != m_IsOnWall && m_Velocity.y > 0.f };
	if (isOnLedge)
	{
		HopUpLedge();
	}

	if (m_IsOnWall && !m_IsOnGround)
	{
		HandleClimbing();
	}
	else if (m_IsOnGround && !m_IsOnWall)
	{
		HandleWalking();
	}
	else if (m_IsOnGround && m_IsOnWall)
	{
		HopOnWall();
		ClimbUp();
	}

	HandleSprites();
	UpdateSprite(elapsedSec);
	if (m_IsOnWall == false) 
	{
		ApplyGravity(elapsedSec);
	}
	HandleLevelCollisions();
	SetIsOnGround();
}

void Bounder::DoChecksOnKirby(Kirby* pKirby)
{
	m_KirbyShape = pKirby->GetShape();
}

void Bounder::Reset()
{
	Enemy::Reset();
	m_IsFirstUpdateCall = true;
}

void Bounder::InitializeSprites()
{
	m_pSprites.push_back(new Sprite{ 1, 0.0f, "bounder_climbing_idle" });
	m_pSprites.push_back(new Sprite{ 2, 0.4f, "bounder_climbing" });
	m_pSprites.push_back(new Sprite{ 2, 0.3f, "bounder_walking" });
}

void Bounder::HandleSprites()
{
	const float movementDetectionTreshold{ 1.f };
	if (m_IsOnWall && abs(m_Velocity.y) > movementDetectionTreshold)
	{
		m_pCurrentSprite = GetSpritePtr("bounder_climbing");
	}
	else if (m_IsOnWall)
	{
		m_pCurrentSprite = GetSpritePtr("bounder_climbing_idle");
	}
	else
	{
		m_pCurrentSprite = GetSpritePtr("bounder_walking");
	}
}

void Bounder::SetStationary()
{
	m_Velocity.x = 0.f;
	m_Velocity.y = 0.f;
}

void Bounder::CheckBothWalls()
{
	m_IsOnWall = IsOnWall();
	if (m_IsOnWall == false)
	{
		ChangeDirection();
		m_IsOnWall = IsOnWall();
	}
}

bool Bounder::IsOnWall()
{
	return LevelManager::GetCurrentLevel()->IsAgainstWall(m_Shape, m_XDirection);
}

void Bounder::HandleClimbing()
{
	const float farTreshold{ 32.f };
	const bool isFarAboveKirby{ m_KirbyShape.bottom + farTreshold < m_Shape.bottom };
	const bool isFarBelowKirby{ m_Shape.bottom + farTreshold < m_KirbyShape.bottom };
	const bool isCloseToKirby{ abs(m_KirbyShape.bottom - m_Shape.bottom) < farTreshold };

	if (isFarAboveKirby)
	{
		if (OddsOfOneIn(300))
		{
			JumpTowardsKirby();
		}
		else
		{
			ClimbDown();
		}
	}
	else if (isFarBelowKirby)
	{
		ClimbUp();
	}
	else if (isCloseToKirby && OddsOfOneIn(1000))
	{
		JumpTowardsKirby();
	}
	else if (OddsOfOneIn(100))
	{
		RemainInPlace();
	}
	else if (OddsOfOneIn(50))
	{
		ClimbUp();
	}
}

void Bounder::HandleWalking()
{
	m_Velocity.x = m_BaseVelocity.x;
	if (OddsOfOneIn(500))
	{
		ThrowStar();
	}
	else if (OddsOfOneIn(100))
	{
		ChangeDirection();
	}
}

void Bounder::ThrowStar()
{
	const float dimSize{ 8.f };
	Rectf spawnRect{};
	spawnRect.left = m_XDirection > 0.f ? m_Shape.left + m_Shape.width / 2 : m_Shape.left;
	spawnRect.bottom = m_Shape.bottom + m_Shape.height / 2;
	spawnRect.width = dimSize;
	spawnRect.height = dimSize;
	m_pProjectileManager->Add(new ThrowingStar{ spawnRect, m_XDirection });
}

void Bounder::JumpTowardsKirby()
{
	m_Velocity.x = abs(m_Shape.left - m_KirbyShape.left);
	m_Velocity.y = 2 * m_BaseVelocity.y;
	ChangeDirection();
	m_Shape.left += m_XDirection;
}

void Bounder::ClimbUp()
{
	m_Velocity.y = m_BaseVelocity.y;
}

void Bounder::ClimbDown()
{
	m_Velocity.y = -m_BaseVelocity.y;
}

void Bounder::HopOnWall()
{
	m_Shape.bottom += 1.f;
}

void Bounder::RemainInPlace()
{
	m_Velocity.x = 0.f;
	m_Velocity.y = 0.f;
}

void Bounder::HopUpLedge()
{
	m_Shape.bottom += m_Shape.height / 2;
	m_Shape.left += 3 * m_XDirection;
}

bool Bounder::OddsOfOneIn(int number)
{
	return m_RandomValue % number == 0;
}

void Bounder::GenerateNumToCheckOdds()
{
	m_RandomValue = rand();
}
