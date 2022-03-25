#pragma once
#include "Projectile.h"
class Star final : public Projectile
{
public:
	Star(Rectf starRect, float xDirection);
	~Star();

	virtual void Update(float elapsedSec) override;
	
private:
	const float m_MaxTime;
	float m_LifeTime;
};

