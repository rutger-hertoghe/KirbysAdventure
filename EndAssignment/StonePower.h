#pragma once
#include "PowerUp.h"

class StonePower final : public PowerUp
{
public:
	explicit StonePower(Actor* pOwner);

	virtual void OnKeyDownEvent() override;
	virtual void ContinuousKeyEvent() override;
	virtual void OnKeyUpEvent() override;

	virtual void Update(float elapsedSec) override;

	virtual bool IsActive() const override;
	virtual std::string GetPowerSuffix() const override;

private:
	bool m_IsActorOnGround;
	bool m_WasActorAlreadyOnGround;
};

