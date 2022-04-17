#pragma once
class Level;
class HUD;
class ObjectManager;
class Kirby;

class Camera final
{
public:
	Camera(float startLocationX, float startLocationY, float windowWidth, float windowHeight, HUD* pHud, ObjectManager* pObjectManager);
	Camera(const Point2f& location, float windowWidth, float windowHeight, HUD* pHud, ObjectManager* pObjectManager);

	static void SetShake();

	void Transform(float distanceFactor = 1.f);
	void Update(float elapsedSec, Kirby* pKirby);

	void TransformHUD();
	void UpdateBoundaries(const Rectf& levelBounds);

	Point2f GetLocation();
	Rectf GetVisibleArea();

private:
	static bool m_IsShaking;

	const float m_HudHeight;
	const float m_MaxShakeTime;
	float m_ShakeTimer;

	int m_XShake;
	int m_YShake;

	Point2f m_Location;
	Point2f m_WindowSize;
	Point2f m_ScalingFactor;
	Rectf m_LevelBoundaries;
	Rectf m_VisibleArea;

	void CalculateVisibleArea();
	void UpdateLocation(float x, float y, float elapsedSec);
	void CalculateShake();
	void UpdateShake(float elapsedSec);

	// TODO: Implement boss camera locking
};

