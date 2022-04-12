#pragma once
class Level;
class HUD;

class Camera final
{
public:
	Camera(float startLocationX, float startLocationY, float windowWidth, float windowHeight, const Rectf& levelBounds, const HUD& hud);
	Camera(const Point2f& location, float windowWidth, float windowHeight, const Rectf& levelBounds, const HUD& hud);

	void Transform(float distanceFactor = 1.f);
	void Update(float x, float y, float elapsedSec);
	void Update(const Point2f& position, float elapsedSec);

	void TransformHUD();
	void UpdateBoundaries(const Rectf& levelBounds);

	Point2f GetViewDimensions();
	Point2f GetLocation();

private:
	const float m_HudHeight;

	Point2f m_Location;
	Point2f m_WindowSize;
	Point2f m_ScalingFactor;
	Rectf m_LevelBoundaries;

};

