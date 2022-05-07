#pragma once
#include "PowerUp.h"

class ProjectileManager;

class SparkPower final : public PowerUp
{
public:
	explicit SparkPower();

	virtual void OnKeyDownEvent() override;
	virtual void ContinuousKeyEvent() override;
	virtual void OnKeyUpEvent() override;

	virtual bool IsActive() const override;
	virtual std::string GetPowerSuffix() const override;

	virtual void Update(float elapsedSec) override;

private:
	const float m_TimeBetweenSpawns;
	float m_TimeSinceSpawn;

	bool m_CanSpawn;
};

