#pragma once
#include "Item.h"
class PowerStar final: public Item
{
public: 
	explicit PowerStar(const Point2f& location, LevelManager* pLevelManager, ProjectileManager* pProjectileManager);
	PowerStar(const PowerStar& other) = delete;
	PowerStar& operator=(const PowerStar& other) = delete;
	PowerStar(PowerStar&& other) = delete;
	PowerStar& operator=(PowerStar&& other) = delete;
	~PowerStar() = default;

	virtual void Update(float elapsedSec) override;
	void SetDirection(float directionValue);
private:
	void InitializeSprites();
	void UpdateStarFlicker();
};

