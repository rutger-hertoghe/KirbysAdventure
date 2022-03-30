#pragma once
#include "Actor.h"
#include "Kirby.h"
class Enemy : public Actor
{
public:
	explicit Enemy(const Point2f& location);
	Enemy(const Enemy& other) = delete;
	Enemy& operator=(const Enemy& other) = delete;
	virtual ~Enemy() override = default; 

	virtual void Update(float elapsedSec);
	bool CheckCollision(const Rectf& kirbyRect);
	
	bool IsActive() const;
	void Reset();
	void SetActivity(bool active);
	bool HasBeenOffScreen();
	void SetOffScreen(bool offscreen, float direction = 1.f);

protected:
	bool m_IsActive;
	bool m_HasBeenOffScreen;
	Point2f m_StartLocation;

	void CorrectVelocities();
};

