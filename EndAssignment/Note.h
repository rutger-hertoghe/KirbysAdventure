#pragma once
#include "Item.h"

class Kirby;

class Note final : public Item
{
public:
	explicit Note(const Point2f& location, Kirby* pKirby, LevelManager* pLevelManager, ProjectileManager* pProjectileManager);
	Note(const Note& other) = delete;
	Note& operator=(const Note& other) = delete;
	Note(Note&& other) = delete;
	Note& operator=(Note&& other) = delete;
	~Note() = default;

	virtual void Update(float elapsedSec) override;
	virtual void DoCollisionEvent() override;

	void Activate(const Point2f& startLocation, float targetX);

private:
	Kirby* m_pKirby;

	void InitializeSprites();
};

