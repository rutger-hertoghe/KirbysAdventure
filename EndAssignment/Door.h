#pragma once
class Door final
{
public:
	explicit Door(const std::string& levelName, const Point2f& location, const Point2f& exitLocation);
	// No dynamically allocated memory so rule of five implementation is not needed, default copy/move constructors and destructor are fine

	bool IsOnDoor(const Rectf& actorShape);
	std::string GetExitLevelName() const;
	Point2f GetExitLocation() const;

private:
	std::string m_LevelName;
	Point2f m_Location;
	Point2f m_ExitLocation;
};

