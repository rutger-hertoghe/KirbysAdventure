#pragma once
#include "PowerUp.h"

class StonePower final : public PowerUp
{
public:
	explicit StonePower();

	virtual void OnKeyDownEvent(const Rectf& shape, float xDirection) override;
	virtual void ContinuousKeyEvent(const Rectf& shape, float xDirection) override;
	virtual void OnKeyUpEvent(const Rectf& shape, float xDirection) override;

	virtual void Update(float elapsedSec) override;

	virtual bool IsActive() const override;
	virtual std::string GetPowerSuffix() const override;

private:
	bool m_IsActorOnGround;
	bool m_WasActorAlreadyOnGround;

	Rectf m_ActorShape;
};

