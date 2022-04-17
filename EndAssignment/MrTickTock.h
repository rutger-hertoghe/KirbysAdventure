#pragma once
#include "Actor.h"
class MrTickTock final : public Actor
{
public:
	explicit MrTickTock(const Point2f& location);
	MrTickTock(const MrTickTock& other) = delete;
	MrTickTock& operator=(const MrTickTock& other) = delete;
	MrTickTock(MrTickTock&& other) = delete;
	MrTickTock& operator=(MrTickTock&& other) = delete;
	~MrTickTock() override;

	virtual void Update(float elapsedSec) override;

private:
	int m_Sprite;
	Point2f m_CameraLockLocation;

	void InitializeSprites();
};

