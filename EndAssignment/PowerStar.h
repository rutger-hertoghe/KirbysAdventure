#pragma once
#include "Item.h"
class PowerStar final: public Item
{
public: 
	PowerStar(const Point2f& location);
	// DO INTERMEDIATE & ABSTRACT CLASSES REQUIRE RULE OF THREE?
	PowerStar(const PowerStar& other) = delete;
	PowerStar& operator=(const PowerStar& other) = delete;
	~PowerStar();

	virtual void Update(float elapsedSec) override;

private:
	
};

