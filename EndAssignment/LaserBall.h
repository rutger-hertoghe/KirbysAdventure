#pragma once
#include "Enemy.h"
class LaserBall final: public Enemy
{
public:
	explicit LaserBall(const Point2f& location, LevelManager* pLevelManager, ProjectileManager* pProjectileManager);
	LaserBall(const LaserBall& other) = delete;
	LaserBall& operator=(const LaserBall& other) = delete;
	LaserBall(LaserBall&& other) = delete;
	LaserBall& operator=(LaserBall&& other) = delete;
	~LaserBall() override = default;

	virtual void Update(float elapsedSec) override;
	virtual void DoChecksOnKirby(Kirby * pKirby) override;
	virtual void Reset() override;

private:
	bool m_IsReboundingY; // He's been through a rough breakup okay
	bool m_IsInPlaceX;
	bool m_IsInPlaceY;
	bool m_IsLocked;

	int m_Charges;

	virtual void InitializeSprites() override;
	void LockIfReady();
	void UpdateTimerOrFlyAway(float elapsedSec);
	void FireIfNecessary();
	void ChangePowerSprites();
	
	void MoveOverY(float velocityIncrease, const Rectf& kirbyShape);
	void MoveOverX(float velocityIncrease, const Rectf& kirbyShape);
};

