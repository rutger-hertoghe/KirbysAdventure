#include "pch.h"
#include "Item.h"

Item::Item(bool effectOnCollision, bool causesBloat, bool persistsOffscreen)
	: Actor{}
	, m_IsBloatItem{ causesBloat }
	, m_HasCollisionEvent{ effectOnCollision }
	, m_IsPersistentOffScreen{persistsOffscreen}
	, m_IsRemoved{false}
{
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

bool Item::IsRemoved() const
{
	return m_IsRemoved;
}

bool Item::IsPersistent() const
{
	return m_IsPersistentOffScreen;
}
