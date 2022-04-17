#pragma once
#include "Actor.h"

class Level;

class Item : public Actor
{
public:
	explicit Item(bool effectOnCollision, bool removedOnCollision, bool isSolid, bool isDestructible, bool causesBloat, bool persistsOffscreen);
	Item(const Item& other) = delete;
	Item& operator=(const Item& other) = delete;
	Item(Item&& other) = delete;
	Item& operator=(Item&& other) = delete;
	virtual ~Item() override;

	virtual void Update(float elapsedSec) override = 0;
	virtual void DoCollisionEvent();
	void Remove();

	bool IsBloatItem() const;
	bool HasCollisionEvent() const;
	bool IsRemovedOnCollision() const;
	bool IsRemoved() const;
	bool IsPersistent() const;
	bool IsSolid() const;
	bool IsDestructible() const;
	bool NeedsToBeDrawn() const;

protected:
	bool m_IsBloatItem;
	bool m_HasCollisionEvent;
	bool m_IsPersistentOffScreen;
	bool m_IsRemovedOnCollision;
	bool m_IsSolid;
	bool m_IsDestructible;
	bool m_IsRemoved;

	bool m_NeedsToBeDrawn;
};

