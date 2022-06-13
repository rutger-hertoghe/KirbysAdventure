#pragma once
#include "Projectile.h"
class Puff final : public Projectile
{
public:
	explicit Puff(Actor* pOwner, Rectf puffRect, float xDirection);
	// No dynamically allocated memory so rule of five implementation is not needed, default copy/move constructors and destructor are fine 

	virtual void Update(float elapsedSec) override;
private:
	const float m_MaxDistance;
	float m_FlownDistance;
};

