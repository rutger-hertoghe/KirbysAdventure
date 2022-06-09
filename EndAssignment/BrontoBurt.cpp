#include "pch.h"
#include "BrontoBurt.h"
#include "Sprite.h"

BrontoBurt::BrontoBurt(const Point2f& location, LevelManager* pLevelManager, ProjectileManager* pProjectileManager)
	: Enemy(location, pLevelManager, pProjectileManager)
	, m_TargetY{location.y}
	, m_Apex{location.y + 100.f}
{
	SetBaseVelocity(40.f, 0.f);

	InitializeSprites();
	m_XDirection = 1.f;
}

void BrontoBurt::Update(float elapsedSec)
{
	AnimateOnlyIfGoingUp(elapsedSec);
	SlowFall(elapsedSec);
	BoostUpIfLowEnough();	
	CorrectApex();
	
	ApplyVelocities(elapsedSec, m_Velocity.x * m_XDirection, m_Velocity.y);
}

void BrontoBurt::DoChecksOnKirby(Kirby* pKirby)
{
	const float kirbyY{ pKirby->GetShape().bottom + pKirby->GetShape().height};

	if (kirbyY < m_TargetY || kirbyY > m_Apex)
	{
		m_TargetY = kirbyY;
	}
}

void BrontoBurt::InitializeSprites()
{
	m_pSprites.push_back(new Sprite{ 2, 0.15f, "brontoburt" });
	SetInitialSprite();
	SetDimsFromSprite();
}

void BrontoBurt::AnimateOnlyIfGoingUp(float elapsedSec)
{
	if (m_Velocity.y > 0.f)
	{
		UpdateSprite(elapsedSec);
	}
	else
	{
		m_CurrentFrame = 0; // Only do animation when going up
	}
}

void BrontoBurt::SlowFall(float elapsedSec)
{
	const float gravityModifier{ 0.3f };
	m_Velocity.y += m_Gravity * gravityModifier * elapsedSec;
}

void BrontoBurt::BoostUpIfLowEnough()
{
	const float velocityBoostTreshold{ -40.f };
	const float boostStrength{ 100.f };
	const float downwardVelocityCap{ -50.f };
	if (m_Velocity.y < velocityBoostTreshold && m_Shape.bottom < m_TargetY)
	{
		m_Velocity.y = boostStrength;
	}
	else if (m_Velocity.y < downwardVelocityCap)
	{
		m_Velocity.y = downwardVelocityCap;
	}
}

void BrontoBurt::CorrectApex()
{
	const float nearZeroTreshold{ 10.f };
	if (m_Velocity.y < nearZeroTreshold && m_Velocity.y > 0.f)
	{
		m_Apex = m_Shape.bottom;
	}
}

