#pragma once
#include "Enemy.h"

class LevelManager;
class ProjectileManager; 

class Bounder final : public Enemy
{
public:
	explicit Bounder(const Point2f& location, LevelManager* pLevelManager, ProjectileManager* pProjectileManager);
	Bounder(const Bounder& other) = delete;
	Bounder& operator=(Bounder& other) = delete;
	Bounder(Bounder&& other) = delete;
	Bounder& operator=(Bounder&& other) = delete;
	~Bounder() override = default;

	virtual void Update(float elapsedSec) override;
	virtual void DoChecksOnKirby(Kirby* pKirby) override;
	virtual void Reset() override;

private:
	bool m_IsOnWall;
	bool m_IsFirstUpdateCall;
	int m_RandomValue;
	Rectf m_KirbyShape;

	virtual void InitializeSprites() override;
	void HandleSprites();

	void SetStationary();
	void CheckBothWalls();
	bool IsOnWall();

	void HandleClimbing();
	void HandleWalking();

	void ThrowStar();
	void JumpTowardsKirby();
	void ClimbUp();
	void ClimbDown();
	void HopOnWall();
	void RemainInPlace();
	void HopUpLedge();

	bool OddsOfOneIn(int number);
	void GenerateNumToCheckOdds();
};

