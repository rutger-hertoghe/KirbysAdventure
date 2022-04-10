#include "pch.h"
#include "Sparky.h"
#include "Sprite.h"
#include "SparkPower.h"

Sparky::Sparky(const Point2f& location)
	: Enemy{location}
{
	InitializeSprites();
	SetBaseVelocity(Point2f{ 30.f, 150.f });
}

void Sparky::Update(float elapsedSec)
{
	// TODO: implement power up routine
	if (m_IsOnGround && m_pCurrentSprite->GetName() == "sparky_jump")
	{
		m_pCurrentSprite = GetSpritePtr("sparky");
		m_Velocity.x = 0.f;
	}
	else if (m_pCurrentSprite->GetName() == "sparky" && m_HasLooped)
	{
		m_pCurrentSprite = GetSpritePtr("sparky_jump");
		m_Velocity.x = m_BaseVelocity.x;
		m_Velocity.y = m_BaseVelocity.y;
	}
	StandardUpdateSequence(elapsedSec);
}

void Sparky::Reset()
{
	Enemy::Reset();
}

void Sparky::InitializePowerUp()
{
	SetPowerUp(new SparkPower{ m_pProjectileManager });
}

void Sparky::DoAbilityCheck(Kirby* pKirby)
{
}

void Sparky::InitializeSprites()
{
	m_pSprites.push_back(new Sprite{ 2, 0.2f, "sparky" });
	m_pSprites.push_back(new Sprite{ 2, 0.1f, "sparky_ability" });
	m_pSprites.push_back(new Sprite{ 1, 0.f, "sparky_jump" });

	SetInitialSprite();
	SetDimsFromSprite();
}
