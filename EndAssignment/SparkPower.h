#pragma once
#include "PowerUp.h"

class ProjectileManager;

class SparkPower final : public PowerUp
{
public:
	explicit SparkPower(ProjectileManager* pProjectileManager);

	virtual void OnKeyDownEvent(const Rectf& shape, float xDirection) override;
	virtual void ContinuousKeyEvent(const Rectf& shape, float xDirection) override;
	virtual void OnKeyUpEvent(const Rectf& shape, float xDirection) override;

	virtual bool IsActive() const override;
	virtual std::string GetPowerSuffix() const;

	virtual void Update(float elapsedSec) override;

private:
	const float m_TimeBetweenSpawns;
	float m_TimeSinceSpawn;

	bool m_CanSpawn;
};

