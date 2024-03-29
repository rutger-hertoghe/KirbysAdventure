#include "pch.h"
#include "Item.h"
#include "Sprite.h"

Item::Item(LevelManager* pLevelManager, ProjectileManager* pProjectileManager, bool effectOnCollision, bool removedOnCollision, bool isSolid, bool isDestructible, bool causesBloat, bool persistsOffscreen)
	: Actor{ pLevelManager, pProjectileManager }
	, m_IsBloatItem{ causesBloat }
	, m_HasCollisionEvent{ effectOnCollision }
	, m_IsPersistentOffScreen{persistsOffscreen}
	, m_IsRemovedOnCollision{ removedOnCollision }
	, m_IsDestructible{isDestructible}
	, m_IsSolid{isSolid}
	, m_IsRemoved{false}
	, m_NeedsToBeDrawn{true}
{
}

Item::~Item()
{
	for (Sprite*& pSprite : m_pSprites)
	{
		delete pSprite;
		pSprite = nullptr;
	}
	m_pCurrentSprite = nullptr;
}

void Item::DoCollisionEvent()
{
}

void Item::Remove()
{
	m_IsRemoved = true;
}

bool Item::IsBloatItem() const
{
	return m_IsBloatItem;
}

bool Item::HasCollisionEvent() const
{
	return m_HasCollisionEvent;
}

bool Item::IsRemovedOnCollision() const
{
	return m_IsRemovedOnCollision;
}

bool Item::IsRemoved() const
{
	// When updating an instance of 'Note' in a certain situation causes an error, I've spent many hours
	// trying to fix it, but I can not for the life of my find why this error occurs and
	// crashes the program
	// The error occurs when manoeuvring kirby underneath a note, while he has a powerup
	return m_IsRemoved;
}

bool Item::IsPersistent() const
{
	return m_IsPersistentOffScreen;
}

bool Item::IsSolid() const
{
	return m_IsSolid;
}

bool Item::IsDestructible() const
{
	return m_IsDestructible;
}

bool Item::NeedsToBeDrawn() const
{
	return m_NeedsToBeDrawn;
}
