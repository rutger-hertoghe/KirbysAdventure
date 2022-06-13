#pragma once
#include "Actor.h"
#include "Kirby.h"

class LevelManager;

class Enemy : public Actor
{
public:
	explicit Enemy(const Point2f& location, LevelManager* pLevelManager, ProjectileManager* pProjectileManager);
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
	void SetOffScreen(bool offscreen);
	void SetDirection(float direction );
	virtual void DoChecksOnKirby(Kirby* kirbyPtr);
	virtual void InitializePowerUp();

protected:
	bool m_CanJump;
	bool m_IsActive;
	bool m_HasBeenOffScreen;
	Point2f m_StartLocation;

	virtual void InitializeSprites() = 0;
	void DeleteSprites();

	void StandardUpdateSequence(float elapsedSec);
	void Jump(float jumpStrength);

	float GetSquaredDistanceToActor(Actor* actor);
};

