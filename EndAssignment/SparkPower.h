#pragma once
#include "PowerUp.h"

class SparkPower final : public PowerUp
{
public:
	SparkPower();
	virtual void OnKeyDownEvent(const Rectf& shape, float xDirection) override;
	virtual void ContinuousKeyEvent(const Rectf& shape, float xDirection) override;
	virtual void OnKeyUpEvent(const Rectf& shape, float xDirection) override;

	virtual bool IsActive() const override;
	virtual std::string GetPowerSuffix() const;

	virtual void Update(float elapsedSec) override;
};

