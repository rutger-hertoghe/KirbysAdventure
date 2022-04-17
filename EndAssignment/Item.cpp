#include "pch.h"
#include "Item.h"
#include "Sprite.h"

Item::Item(bool effectOnCollision, bool removedOnCollision, bool isSolid, bool isDestructible, bool causesBloat, bool persistsOffscreen)
	: Actor{}
	, m_IsBloatItem{ causesBloat }
	, m_HasCollisionEvent{ effectOnCollision }
	, m_IsPersistentOffScreen{persistsOffscreen}
	, m_IsRemovedOnCollision{ removedOnCollision }
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
