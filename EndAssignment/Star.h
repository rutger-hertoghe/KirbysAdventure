#pragma once
#include "Projectile.h"

class Star final : public Projectile
{
public:
	explicit Star(Actor* pOwner, Rectf starRect, float xDirection);
	// No dynamically allocated memory so rule of five implementation is not needed, default copy/move constructors and destructor are fine 

	virtual void Update(float elapsedSec) override;
};

