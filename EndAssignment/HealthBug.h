#pragma once
#include "Item.h"

class Kirby;

class HealthBug final : public Item
{
public:
	explicit HealthBug(const Point2f& location, Kirby* pKirby, LevelManager* pLevelManager, ProjectileManager* pProjectileManager);
	HealthBug(const HealthBug& other) = delete;
	HealthBug& operator=(const Item& other) = delete;
	HealthBug(HealthBug&& other) = delete;
	HealthBug& operator=(HealthBug&& other) = delete;
	virtual ~HealthBug() override = default;

	virtual void Update(float elapsedSec) override;
	virtual void DoCollisionEvent() override;

private:
	Kirby* m_pKirby;

	void InitializeSprites();
};

