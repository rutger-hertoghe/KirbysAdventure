#pragma once
#include "PowerUp.h"

class ProjectileManager;

class FirePower final : public PowerUp
{
public:
	explicit FirePower(Actor* pOwner);

	virtual void OnKeyDownEvent() override;
	virtual void ContinuousKeyEvent() override;
	virtual void OnKeyUpEvent() override;
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

