#pragma once
#include "Enemy.h"

class LevelManager;

class BrontoBurt final : public Enemy
{
public:
	explicit BrontoBurt(const Point2f& location, LevelManager* pLevelManager, ProjectileManager* pProjectileManager);
	BrontoBurt(const BrontoBurt& other) = delete;
	BrontoBurt& operator=(const BrontoBurt& other) = delete;
	BrontoBurt(BrontoBurt&& other) = delete;
	BrontoBurt& operator=(BrontoBurt&& other) = delete;
	~BrontoBurt() override = default;

	virtual void Update(float elapsedSec) override;
	virtual void DoChecksOnKirby(Kirby* pKirby) override;
private:
	float m_TargetY;
	float m_Apex;
	virtual void InitializeSprites() override;
	void AnimateOnlyIfGoingUp(float elapsedSec);
	void SlowFall(float elapsedSec);
	void BoostUpIfLowEnough();
	void CorrectApex();
};

