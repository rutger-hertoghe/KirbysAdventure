#pragma once
#include "Enemy.h"
class WaddleDee final : public Enemy
{
public:
	explicit WaddleDee(const Point2f& location);
	WaddleDee(const WaddleDee& other) = delete;
	WaddleDee& operator=(const WaddleDee& other) = delete;
	WaddleDee(WaddleDee&& other) = delete;
	WaddleDee& operator=(WaddleDee&& other) = delete;
	~WaddleDee() override = default; 

	virtual void Update(float elapsedSec) override;
private:
	
	virtual void InitializeSprites() override;
};

