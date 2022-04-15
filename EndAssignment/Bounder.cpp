#include "pch.h"
#include "Bounder.h"
#include "Sprite.h"
#include "ProjectileManager.h"
#include "ThrowingStar.h"

Bounder::Bounder(const Point2f& location)
	: Enemy{location}
	, m_IsOnWall{false}
	, m_IsFirstUpdateCall{true}
{
	InitializeSprites();
	SetInitialSprite();
	SetDimsFromSprite();
	SetBaseVelocity(50.f, 50.f);
	m_XDirection = 1.f;
}

void Bounder::Update(float elapsedSec)
{
	// TODO: fix bounder movement
	//if (m_IsFirstUpdateCall)
	//{
	//	SetStationary();
	//	// CheckBothWalls();
	//	m_IsFirstUpdateCall = false;
	//}

	ApplyVelocities(elapsedSec, m_XDirection * m_Velocity.x, m_Velocity.y);
	if (m_IsBeingInhaled) return; // If enemy is being inhaled, code below can not be executed

	HandleLevelCollisions();
	SetIsOnGround();

	int randomValue{ rand() };

	m_IsOnWall = IsOnWall();

	if (m_IsOnWall && m_IsOnGround)
	{
		m_Velocity.y = m_BaseVelocity.y;
		m_Velocity.x = 0.f;
		m_pCurrentSprite->GetName() = "bounder_climbing";
	}
	else if (m_IsOnGround)
	{
		m_Velocity.x = m_BaseVelocity.x;
	}

	if (m_IsOnGround && (m_pCurrentSprite->GetName() == "bounder_climbing" || m_pCurrentSprite->GetName() == "bounder_climbing_idle"))
	{
		m_pCurrentSprite = GetSpritePtr("bounder_walking");
	}

	if (m_IsOnGround && randomValue % 200 == 0)
	{
		m_pProjectileManager->Add(new ThrowingStar{ m_Shape, m_XDirection });
	}
	else if (m_IsOnGround && randomValue % 100 == 0)
	{
		ChangeDirection();
	}

	UpdateSprite(elapsedSec);
	if (m_IsOnWall == false) 
	{
		ApplyGravity(elapsedSec);
	}
	
}

void Bounder::DoAbilityCheck(Kirby* pKirby)
{
	Rectf kirbyShape{ pKirby->GetShape() };

	/*if (kirbyShape.bottom <= m_Shape.bottom && m_IsOnWall)
	{
		if (rand() % 100 == 0)
		{
			m_Velocity.x = abs(kirbyShape.left - m_Shape.left);
			m_Velocity.y = m_BaseVelocity.y;
			ChangeDirection();
			m_pCurrentSprite = GetSpritePtr("bounder_walking");
		}
		else 
		{
			m_Velocity.y = -m_BaseVelocity.y;
			m_pCurrentSprite = GetSpritePtr("bounder_climbing");
		}
	}
	else if (m_IsOnWall)
	{
		if (rand() % 100 == 0)
		{
			m_Velocity.y = m_BaseVelocity.y;
			m_pCurrentSprite = GetSpritePtr("bounder_climbing");
		}
		else if (rand() % 100 == 50)
		{
			m_Velocity.y = 0.f;
			m_pCurrentSprite = GetSpritePtr( "bounder_climbing_idle" );
		}
	}*/
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
	return m_pCurrentLevel->IsAgainstWall(m_Shape, m_XDirection);
}
