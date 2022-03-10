#include "pch.h"
#include "Platform.h"
#include "utils.h"

std::vector<Point2f> Platform::GetRectVertices() const
{
	std::vector<Point2f> vector{};
	vector.push_back(Point2f{ m_Rect.left, m_Rect.bottom });
	vector.push_back(Point2f{ m_Rect.left, m_Rect.bottom + m_Rect.height});
	vector.push_back(Point2f{ m_Rect.left + m_Rect.width, m_Rect.bottom + m_Rect.height});
	vector.push_back(Point2f{ m_Rect.left + m_Rect.width, m_Rect.bottom});
	return vector;
}


Platform::Platform(Rectf rect)
	: m_Rect{rect}
{
}

void Platform::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	Point2f rayStart{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height };
	Point2f rayEnd{ actorShape.left + actorShape.width / 2, actorShape.bottom };

	utils::HitInfo hitInfo{};
	if (utils::Raycast(GetRectVertices(), rayStart, rayEnd, hitInfo))
	{
		actorShape.bottom = hitInfo.intersectPoint.y;
		actorVelocity.y = 0;
	}
}

bool Platform::IsOnGround(Rectf& actorShape) const
{
	Point2f rayStart{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height };
	Point2f rayEnd{ actorShape.left + actorShape.width / 2, actorShape.bottom - 1.f};

	utils::HitInfo hitInfo{};
	if (utils::Raycast(GetRectVertices(), rayStart, rayEnd, hitInfo))
	{
		return true;
	}
	return false;
}