#pragma once
#include "Projectile.h"
#include "Sprite.h"

class Fireball final : public Projectile
{
public:
	explicit Fireball(Actor* pOwner, const Rectf& fireballRect, const Vector2f& directionVector);
	// No dynamically allocated memory so rule of five implementation is not needed, default copy/move constructors and destructor are fine 

	virtual void Update(float elapsedSec) override;

private:
	const float m_MaxDistance;
	float m_FlownDistance;
};

