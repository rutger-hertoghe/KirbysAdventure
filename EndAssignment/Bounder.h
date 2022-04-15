#pragma once
#include "Enemy.h"
class Bounder final : public Enemy
{
public:
	explicit Bounder(const Point2f& location);
	Bounder(const Bounder& other) = delete;
	Bounder& operator=(Bounder& other) = delete;
	Bounder(Bounder&& other) = delete;
	Bounder& operator=(Bounder&& other) = delete;
	~Bounder() override = default;

	virtual void Update(float elapsedSec) override;
	virtual void DoAbilityCheck(Kirby* pKirby) override;
	virtual void Reset() override;

private:
	bool m_IsOnWall;
	bool m_IsFirstUpdateCall;

	virtual void InitializeSprites() override;

	void SetStationary();
	void CheckBothWalls();
	bool IsOnWall();
};

