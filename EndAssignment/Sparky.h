#pragma once
#include "Enemy.h"

class Sparky final : public Enemy
{
public:
	explicit Sparky(const Point2f& location);
	Sparky(const Sparky& other) = delete;
	Sparky& operator=(const Sparky& other) = delete;
	Sparky(Sparky&& other) = delete;
	Sparky& operator=(Sparky&& other) = delete;
	~Sparky() override = default;

	virtual void Update(float elapsedSec) override;

	virtual void DoChecksOnKirby(Kirby* pKirby) override;

private:
	bool m_IsPowerUsable;
	bool m_IsInert;

	virtual void InitializeSprites() override;
	virtual void InitializePowerUp() override;
	void HandleJumping();
	void UpdatePowerState(float elapsedSec);
};

