#pragma once
#include "Item.h"

class Kirby;

class OneUp final : public Item
{
public:
	explicit OneUp(const Point2f& location, Kirby* pKirby);
	OneUp(const OneUp& other) = delete;
	OneUp& operator=(const Item& other) = delete;
	OneUp(OneUp&& other) = delete;
	OneUp& operator=(OneUp&& other) = delete;
	virtual ~OneUp() override = default;

	virtual void Update(float elapsedSec) override;
	virtual void DoCollisionEvent() override;

private:
	Kirby* m_pKirby;

	void InitializeSprites();
};

