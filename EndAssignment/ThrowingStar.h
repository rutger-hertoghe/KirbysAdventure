#pragma once
#include "Projectile.h"

class ThrowingStar final : public Projectile
{
public:
	explicit ThrowingStar(Actor* pOwner, Rectf starRect, float xDirection);

	virtual void Update(float elapsedSec) override;

private:
	const float m_MaxExistenceTime;
	float m_ExistenceTime;
};

