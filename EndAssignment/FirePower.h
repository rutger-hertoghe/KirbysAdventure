#pragma once
#include "PowerUp.h"

class ProjectileManager;

class FirePower final : public PowerUp
{
public:
	explicit FirePower(ProjectileManager* pProjectileManager);

	virtual void OnKeyDownEvent(const Rectf& shape, float xDirection) override;
	virtual void ContinuousKeyEvent(const Rectf& shape, float xDirection) override;
	virtual void OnKeyUpEvent(const Rectf& shape, float xDirection) override;
	virtual void Update(float elapsedSec) override;

	virtual bool IsActive() const override;
	virtual std::string GetPowerSuffix() const;

private:
	const float m_TimeBetweenSpawns;
	float m_TimeSinceSpawn;

	int m_YDirection;
	int m_AngleDifference;

	bool m_CanSpawn;

	void ChangeFireAngle();
	
};

