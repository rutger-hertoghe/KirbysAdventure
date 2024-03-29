#pragma once
#include "Enemy.h"

class LevelManager;

class HotHead final : public Enemy
{
public:
	explicit HotHead(const Point2f& location, LevelManager* pLevelManager, ProjectileManager* pProjectileManager);
	HotHead(const HotHead& other) = delete;
	HotHead& operator=(const HotHead& other) = delete;
	HotHead(HotHead&& other) = delete;
	HotHead& operator=(HotHead&& other) = delete;
	~HotHead() override = default;

	virtual void Update(float elapsedSec) override;

	virtual void Reset() override;
	virtual void DoChecksOnKirby(Kirby* pKirby) override;

private:
	bool m_IsUsingPower;
	bool m_IsPowerUsable;
	bool m_IsInert;

	virtual void InitializeSprites() override;
	virtual void InitializePowerUp() override;
	void UpdatePowerState(float elapsedSec, float maxPowerTime, float powerExecutionStart, float maxInertiaTime);
};

