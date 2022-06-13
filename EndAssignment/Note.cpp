#include "pch.h"
#include "Note.h"
#include "Sprite.h"
#include "ObjectManager.h"
#include "Kirby.h"

Note::Note(const Point2f& location, Kirby* pKirby, LevelManager* pLevelManager, ProjectileManager* pProjectileManager)
	: Item{ pLevelManager, pProjectileManager, true, false, false, false, true, false }
	, m_pKirby{pKirby}
{
	m_IsRemoved = true;
	SetLocation(location);
	InitializeSprites();
	SetInitialSprite();
	SetDimsFromSprite();
	// Target X and location are decided to determine velocity and its sign,
	// seeing that the target X is incrementally further from ticktock on every note launched
}

void Note::Update(float elapsedSec)
{
	ApplyVelocities(elapsedSec, m_XDirection * m_Velocity.x, m_Velocity.y);

	if (m_IsBeingInhaled)
	{
		return;
	}

	ApplyGravity(elapsedSec);
	HandleLevelCollisions();
	SetIsOnGround();
	if (m_IsOnGround)
	{	
		Remove();
		SoundFXManager::Play("notehit");
		ObjectManager::GetObjectMngr()->AddRemovalFX(GetLocation(), RemovalFX::FXType::item);
	}
}

void Note::DoCollisionEvent()
{
	if (m_IsBeingInhaled)
	{
		return;
	}
	m_pKirby->DecrementHealth(m_pKirby->GetRelativeDirection(this));
	SoundFXManager::Play("notehit");
}

void Note::Activate(const Point2f& startLocation, float targetX)
{
	m_IsRemoved = false;
	SetLocation(startLocation);
	SetBaseVelocity(targetX - startLocation.x, 200.f);
}

void Note::InitializeSprites()
{
	m_pSprites.push_back(new Sprite{1, 0.f, "note"});
}
