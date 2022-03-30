#pragma once
#include "PowerUp.h"

class FirePower final : public PowerUp
{
public:
	FirePower();

	virtual void OnKeyDownEvent(const Rectf& shape, float xDirection) override;
	virtual void ContinuousKeyEvent(const Rectf& shape, float xDirection) override;
	virtual void OnKeyUpEvent(const Rectf& shape, float xDirection) override;
	virtual void Update(float elapsedSec) override;

	virtual bool IsActive() const override;
	virtual std::string GetPowerSuffix() const;

private:
	Rectf m_Rect;
	const float m_TimeBetweenSpawns;
	float m_TimeSinceSpawn;

	int m_YDirection;

	bool m_CanSpawn;

	int m_AngleDifference;
	void ChangeFireAngle();
	
};

