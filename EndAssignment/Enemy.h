#pragma once
#include "Actor.h"
#include "Kirby.h"
class Enemy : public Actor
{
public:
	explicit Enemy(const Point2f& location);
	Enemy(const Enemy& other) = delete;
	Enemy& operator=(const Enemy& other) = delete;
	Enemy(Enemy&& other) = delete;
	Enemy& operator=(Enemy&& other) = delete;
	virtual ~Enemy() override; 

	virtual void Update(float elapsedSec) = 0;
	
	bool IsActive() const;
	virtual void Reset();
	void SetActivity(bool active);
	bool HasBeenOffScreen() const;
	void SetOffScreen(bool offscreen, float direction = 1.f);
	virtual void InitializePowerUp();
	virtual void DoAbilityCheck(Kirby* kirbyPtr);

protected:
	// TODO: Clean states that are not even being used, clean class in general
	enum class ActionState
	{
		dormant,
		moving,
		power_start,
		power_continuous,
		power_end,
		dying
	};

	bool m_CanJump;
	// bool m_IsActive;
	bool m_HasBeenOffScreen;
	Point2f m_StartLocation;

	ActionState m_ActionState;

	void CorrectVelocities();

	virtual void InitializeSprites() = 0;
	void DeleteSprites();

	void StandardUpdateSequence(float elapsedSec);
	void Jump(float jumpStrength);

	float GetSquaredDistanceToActor(Actor* actor);
};

