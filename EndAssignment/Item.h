#pragma once
#include "Actor.h"
class Item : public Actor
{
public:
	Item();
	// DO INTERMEDIATE & ABSTRACT CLASSES REQUIRE RULE OF THREE?
	Item(const Item& other) = delete;
	Item& operator=(const Item& other) = delete;
	virtual ~Item() override = default;

	virtual void Update(float elapsedSec) override = 0;

private:

};

