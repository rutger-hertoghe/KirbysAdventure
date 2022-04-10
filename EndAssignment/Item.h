#pragma once
#include "Actor.h"

class Level;

class Item : public Actor
{
public:
	explicit Item();
	Item(const Item& other) = delete;
	Item& operator=(const Item& other) = delete;
	Item(Item&& other) = delete;
	Item& operator=(Item&& other) = delete;
	virtual ~Item() override = default;

	virtual void Update(float elapsedSec) override = 0;

private:

};

