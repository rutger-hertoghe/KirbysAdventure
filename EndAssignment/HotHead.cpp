#include "pch.h"
#include "HotHead.h"
#include "Sprite.h"
#include "FirePower.h"

HotHead::HotHead(const Point2f& location)
	: Enemy{ location }
	, m_IsPowerUsable{false}
	, m_IsUsingPower{false}
	, m_IsInert{false}
{
	SetBaseVelocity(50.f, 0.f);

	InitializeSprites();

	m_XDirection = 1.f;
	InitializePowerUp();
}

void HotHead::Update(float elapsedSec)
{
	if (IsBeingInhaled())
	{
		ApplyVelocities(elapsedSec, m_XDirection * m_Velocity.x, m_Velocity.y);
		return;
	}

	const float powerExecutionStart{ 0.5f };
	const float powerDuration{ 2.f };
	const float inertiaDuration{ 3.f };
	const float maxPowerTime{ powerExecutionStart  + powerDuration};
	const float maxInertiaTime{ maxPowerTime + inertiaDuration };

	if (HasPowerUp())
	{
		if (m_IsPowerUsable)
		{
			m_IsPowerUsable = false;
			m_IsInert = true;
			m_pCurrentSprite = GetSpritePtr("hothead_charge");
			m_Velocity.x = 0.f;
		}
		else if (m_ArbitraryTimer > powerExecutionStart && m_IsUsingPower == false)
		{
			m_IsUsingPower = true;
			m_pCurrentSprite = GetSpritePtr("hothead_ability");
		}
		else if (m_ArbitraryTimer > powerExecutionStart && m_ArbitraryTimer < maxPowerTime && m_IsUsingPower)
		{
			m_pPowerUp->ContinuousKeyEvent(m_Shape, m_XDirection);
			m_pPowerUp->Update(elapsedSec);
		}
		else if (m_ArbitraryTimer > maxPowerTime && m_IsUsingPower)
		{
			m_pCurrentSprite = GetSpritePtr("hothead");
			m_Velocity.x = m_BaseVelocity.x;
		}
		else if (m_ArbitraryTimer > maxInertiaTime)
		{
			m_ArbitraryTimer = 0.f;
		}
	}

	if (m_IsInert == true) // Counter should only run when using power or during inertia period
	{
		m_ArbitraryTimer += elapsedSec;
	}

	if (m_ArbitraryTimer < maxPowerTime && m_IsInert)
	{
		std::string spriteName = m_pCurrentSprite->GetName();
		Flicker(m_ArbitraryTimer, spriteName);
	}

	ApplyVelocities(elapsedSec, m_XDirection * m_Velocity.x, m_Velocity.y);
	UpdateSprite(elapsedSec);
	ApplyGravity(elapsedSec);
	HandleLevelCollisions();
	SetIsOnGround();
}

void HotHead::Reset()
{
	Enemy::Reset();
	m_IsPowerUsable = false;
	m_IsUsingPower = false;
	m_IsInert = false;
	m_ArbitraryTimer = 0.f;
}

void HotHead::InitializePowerUp()
{
	SetPowerUp(new FirePower{m_pProjectileManager});
}

void HotHead::InitializeSprites()
{
	m_pSprites.push_back(new Sprite{ 2, 0.3f, "hothead" });
	m_pSprites.push_back(new Sprite{ 1, 0.f, "hothead_charge" });
	m_pSprites.push_back(new Sprite{ 1, 0.f, "hothead_ability"});
	CreateAltSprites();

	SetInitialSprite();
	SetDimsFromSprite();
}

void HotHead::DoAbilityCheck(Kirby* pKirby)
{
	const float squaredDistanceValue {GetSquaredDistanceToActor(pKirby) };
	const float triggerDistance{ 64.f };
	const float sqTriggerDistance{ triggerDistance * triggerDistance };
	const float nearZeroTreshold{ 0.0001f };

	if (squaredDistanceValue < sqTriggerDistance && m_IsInert == false)
	{
		m_IsPowerUsable = true;
	}
}

