#pragma once
#include "Actor.h"

class Level;

class Item : public Actor
{
public:
	explicit Item(bool effectOnCollision, bool causesBloat, bool persistsOffscreen);
	Item(const Item& other) = delete;
	Item& operator=(const Item& other) = delete;
	Item(Item&& other) = delete;
	Item& operator=(Item&& other) = delete;
	virtual ~Item() override = default;

	virtual void Update(float elapsedSec) override = 0;
	virtual void DoCollisionEvent();
	void Remove();

	bool IsBloatItem() const;
	bool HasCollisionEvent() const;
	bool IsRemoved() const;
	bool IsPersistent() const;

protected:
	bool m_IsBloatItem;
	bool m_HasCollisionEvent;
	bool m_IsPersistentOffScreen;
	bool m_IsRemoved;
};
