#pragma once
class Level;
class HUD;
class ObjectManager;
class Kirby;

class Camera final
{
public:
	explicit Camera(float startLocationX, float startLocationY, float windowWidth, float windowHeight, HUD* pHud);
	explicit Camera(const Point2f& location, float windowWidth, float windowHeight, HUD* pHud);
	// No dynamically allocated memory so rule of five implementation is not needed, default copy/move constructors and destructor are fine 
	
	static void SetShake();

	void Transform(float distanceFactor = 1.f);
	void Update(float elapsedSec, Kirby* pKirby);

	void TransformHUD() const;
	void UpdateBoundaries(const Rectf& levelBounds);
	void SetLocked(const Point2f& lockLocation);
	bool IsLocked() const;
	void Unlock();

	Point2f GetLocation() const;
	Rectf GetVisibleArea() const;

private:
	static bool m_IsShaking;

	const float m_HudHeight;
	const float m_MaxShakeTime;
	float m_ShakeTimer;

	int m_XShake;
	int m_YShake;

	bool m_IsLocked;
	bool m_IsUnlocking;

	Point2f m_Location;
	Point2f m_LockLocation;
	Point2f m_WindowSize;
	Point2f m_ScalingFactor;
	Rectf m_LevelBoundaries;
	Rectf m_VisibleArea;

	void CalculateVisibleArea();
	void UpdateLocation(float x, float y);
	void CalculateShake();
	void UpdateShake(float elapsedSec);
	void LockKirbyToVisibleArea(Kirby* pKirby);
	void MoveToLockLocation(float elapsedSec);
	void MoveBackToKirby(float elapsedSec, Kirby* pKirby);
};

