#pragma once
#include "Item.h"

class StarBlock final:
    public Item
{
// Not actually an enemy but much of the desired behaviour is the same, except for damaging kirby, prevention is handled in the objectmanager
public:
	explicit StarBlock(const Point2f& location);
	StarBlock(const StarBlock& other) = delete;
	StarBlock& operator=(const StarBlock& other) = delete;
	StarBlock(StarBlock&& other) = delete;
	StarBlock& operator=(StarBlock&& other) = delete;
	~StarBlock() override = default;

	virtual void Update(float elapsedSec) override;
private:
	virtual void InitializeSprites();
};

