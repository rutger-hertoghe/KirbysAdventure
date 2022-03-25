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
	
	bool IsActive() const;
	void Reset();
	void SetActivity(bool active);
	bool HasBeenOffScreen();
	void SetOffScreen(bool offscreen);

protected:
	bool m_IsActive;
	bool m_HasBeenOffScreen;
	Point2f m_StartLocation;

	void CorrectVelocities();
};

