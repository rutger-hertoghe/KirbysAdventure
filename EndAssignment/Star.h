#pragma once
#include "Projectile.h"

class Star final : public Projectile
{
public:
	explicit Star(Rectf starRect, float xDirection);

	virtual void Update(float elapsedSec) override;
};

