#pragma once
#include "PowerUp.h"

class SparkPower : public PowerUp
{
public:
	SparkPower();
	virtual ~SparkPower();
	virtual void OnKeyDownEvent(Rectf shape, float xDirection) override;
	virtual void ContinuousKeyEvent(Rectf shape, float xDirection) override;
	virtual void OnKeyUpEvent(Rectf shape, float xDirection) override;

	virtual void Draw() const override;

	virtual bool IsActive() const override;
	virtual bool IsOneShot() const override;
	virtual std::string GetPowerSuffix() const;
};

