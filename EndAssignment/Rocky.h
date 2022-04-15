#pragma once
#include "Enemy.h"
class Rocky final : public Enemy
{
public:
	explicit Rocky(const Point2f& location);
	Rocky(const Rocky& other) = delete;
	Rocky& operator=(Rocky& other) = delete;
	Rocky(Rocky&& other) = delete;
	Rocky& operator=(Rocky&& other) = delete;
	~Rocky() override = default;

	virtual void Update(float elapsedSec) override;
	virtual void Reset() override;

private:
	bool m_IsAbilityActive;
	const float m_MaxStateTime;

	virtual void InitializeSprites() override;
	virtual void InitializePowerUp() override;
	void ChangeState();
};

