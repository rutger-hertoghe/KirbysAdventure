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

	virtual void Reset() override;
	virtual void InitializePowerUp() override;
	virtual void DoAbilityCheck(Kirby* pKirby) override;

private:
	virtual void InitializeSprites() override;
};

