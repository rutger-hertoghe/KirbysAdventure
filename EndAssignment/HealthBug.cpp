#include "pch.h"
#include "HealthBug.h"
#include "Sprite.h"
#include "Kirby.h"

HealthBug::HealthBug(const Point2f& location, Kirby* pKirby, LevelManager* pLevelManager, ProjectileManager* pProjectileManager)
	: Item{ pLevelManager, pProjectileManager,  true, true, false, false, false, true }
	, m_pKirby{ pKirby }
{
	InitializeSprites();
	SetInitialSprite();
	SetDimsFromSprite();
	SetLocation(location);
}

void HealthBug::Update(float elapsedSec)
{
	ApplyVelocities(elapsedSec, m_Velocity.x, m_Velocity.y);
	if (m_IsBeingInhaled) return;
	UpdateSprite(elapsedSec);
	HandleLevelCollisions();
	SetIsOnGround();
}

void HealthBug::DoCollisionEvent()
{
	m_pKirby->FullyHeal();
}

void HealthBug::InitializeSprites()
{
	m_pSprites.push_back(new Sprite{ 1, 0.0f, "Healthbug" });
}