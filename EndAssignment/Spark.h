#pragma once
#include "Projectile.h"

class Spark final : public Projectile
{
public:
	explicit Spark(Actor* pOwner, const Rectf& sparkRect, const Vector2f& directionVector);
	// No dynamically allocated memory so rule of five implementation is not needed, default copy/move constructors and destructor are fine 

	virtual void Update(float elapsedSec) override;

private:
};

