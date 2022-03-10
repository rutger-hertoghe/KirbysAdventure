#pragma once
#include "PowerUp.h"

class FirePower : public PowerUp
{
public:
	FirePower();
	virtual ~FirePower() override;
	virtual void OnKeyDownEvent(Rectf shape, float xDirection) override;
	virtual void ContinuousKeyEvent(Rectf shape, float xDirection) override;
	virtual void OnKeyUpEvent(Rectf shape, float xDirection) override;
	virtual void Draw() const override;

	virtual bool IsActive() const override;
	virtual bool IsOneShot() const override;
	virtual std::string GetPowerSuffix() const;

private:
	Rectf m_DamageZone;
};

