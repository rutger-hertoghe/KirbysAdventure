#include "pch.h"
#include "OneUp.h"
#include "Sprite.h"
#include "Kirby.h"

OneUp::OneUp(const Point2f& location, Kirby* pKirby)
	: Item{true, true, false, false, false, true}
	, m_pKirby{pKirby}
{
	InitializeSprites();
	SetInitialSprite();
	SetDimsFromSprite();
	SetLocation(location);
}

void OneUp::Update(float elapsedSec)
{
	ApplyVelocities(elapsedSec, m_Velocity.x, m_Velocity.y);
	if (m_IsBeingInhaled) return;
	UpdateSprite(elapsedSec);
	ApplyGravity(elapsedSec);
	HandleLevelCollisions();
	SetIsOnGround();
}

void OneUp::DoCollisionEvent()
{
	m_pKirby->IncrementLives();
}

void OneUp::InitializeSprites()
{
	m_pSprites.push_back(new Sprite{ 4, 0.2f, "OneUp" });
}
