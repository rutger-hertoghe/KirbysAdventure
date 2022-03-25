#pragma once
#include "Projectile.h"
#include "Sprite.h"

class Fireball final : public Projectile
{
public:
	Fireball(Rectf fireballRect, Vector2f directionVector);
	~Fireball();

	virtual void Update(float elapsedSec) override;

private:
	const float m_MaxDistance;
	float m_FlownDistance;
};

