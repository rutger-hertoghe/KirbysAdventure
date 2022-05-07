#pragma once
#include "Projectile.h"

class Spark final : public Projectile
{
public:
	Spark(Actor* pOwner, const Rectf& sparkRect, const Vector2f& directionVector);

	virtual void Update(float elapsedSec) override;

private:
};

