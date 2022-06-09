#pragma once
#include "Enemy.h"

class LevelManager;

class Parasol final : public Enemy
{
public:
	explicit Parasol(const Point2f& location, LevelManager* pLevelManager, ProjectileManager* pProjectileManager);
	Parasol(const Parasol& other) = delete;
	Parasol& operator=(const Parasol& other) = delete;
	Parasol(Parasol&& other) = delete;
	Parasol& operator=(Parasol&& other) = delete;
	~Parasol() override = default;

	virtual void Update(float elapsedSec) override;
private:

	virtual void InitializeSprites() override;

};

