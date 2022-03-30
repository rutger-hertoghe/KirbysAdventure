#pragma once
#include "Projectile.h"
class Puff final : public Projectile
{
public:
	explicit Puff(Rectf puffRect, float xDirection);

	virtual void Update(float elapsedSec) override;
private:
	const float m_MaxDistance;
	float m_FlownDistance;
};

