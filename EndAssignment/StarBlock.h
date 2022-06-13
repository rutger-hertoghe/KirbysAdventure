#pragma once
#include "Item.h"

class Kirby;

class StarBlock final : public Item
{
public:
	explicit StarBlock(const Point2f& location, Kirby* pKirby, LevelManager* pLevelManager, ProjectileManager* pProjectileManager);
	StarBlock(const StarBlock& other) = delete;
	StarBlock& operator=(const StarBlock& other) = delete;
	StarBlock(StarBlock&& other) = delete;
	StarBlock& operator=(StarBlock&& other) = delete;
	~StarBlock() override = default;

	virtual void Update(float elapsedSec) override;
private:
	virtual void InitializeSprites();
	Kirby* m_pKirby;
};

