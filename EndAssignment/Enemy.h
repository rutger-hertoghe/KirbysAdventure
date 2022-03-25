#pragma once
#include "Actor.h"
#include "Kirby.h"
class Enemy : public Actor
{
public:
	Enemy(const Point2f& location);
	virtual ~Enemy() override;

	virtual void Update(float elapsedSec);
	bool CheckCollision(const Rectf& kirbyRect);

protected:
	bool m_IsActive;

	void CorrectVelocities();
};

