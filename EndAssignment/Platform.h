#pragma once
#include "Vector2f.h"

class Platform
{
public:
	Platform(Rectf rect);
	Platform(float left, float bottom, float width, float height);

	virtual void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	bool IsOnGround(Rectf& actorShape) const;

protected:
	Rectf m_Rect;
	std::vector<Point2f> GetRectVertices() const;
};

