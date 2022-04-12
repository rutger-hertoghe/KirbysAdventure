#include "pch.h"
#include "Door.h"
#include "utils.h"

Door::Door(const std::string& levelName, const Point2f& location, const Point2f& exitLocation)
	: m_LevelName{levelName}
	, m_Location{location}
	, m_ExitLocation{exitLocation}
{
}

bool Door::IsOnDoor(const Rectf& actorShape)
{
	Point2f actorCenter{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2 };
	Rectf doorRect{ m_Location.x, m_Location.y, 16.f, 32.f };
	return utils::IsPointInRect(actorCenter, doorRect);
}

std::string Door::GetExitLevelName() const
{
	return m_LevelName;
}

Point2f Door::GetExitLocation() const
{
	return m_ExitLocation;
}

