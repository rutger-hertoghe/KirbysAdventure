#pragma once
#include "Enemy.h"
class WaddleDee final : public Enemy
{
public:
	explicit WaddleDee(const Point2f& location);
	WaddleDee(const WaddleDee& other) = delete;
	WaddleDee& operator=(const WaddleDee& other) = delete;
	~WaddleDee() override;

	virtual void Update(float elapsedSec) override;
private:
	
};

