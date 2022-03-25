#pragma once
#include "Actor.h"
class PowerStar final: public Actor
{
public: 
	PowerStar(const Point2f& location);
	~PowerStar();

	virtual void Update(float elapsedSec) override;

private:
	
};

