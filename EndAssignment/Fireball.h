#pragma once
#include "Projectile.h"
#include "Sprite.h"

class Fireball final : public Projectile
{
public:
	explicit Fireball(ActorType owner, const Rectf& fireballRect, const Vector2f& directionVector);

	virtual void Update(float elapsedSec) override;

private:
	const float m_MaxDistance;
	float m_FlownDistance;
};

