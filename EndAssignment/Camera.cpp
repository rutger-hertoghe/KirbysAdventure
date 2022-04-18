#include "pch.h"
#include "Camera.h"
#include "Level.h"
#include "HUD.h"
#include "ObjectManager.h"
#include "Kirby.h"

bool Camera::m_IsShaking{ false };

Camera::Camera(float startLocationX, float startLocationY, float windowWidth, float windowHeight, HUD* pHud)
	: m_Location{ startLocationX, startLocationY }
	, m_WindowSize{ windowWidth, windowHeight}
	, m_HudHeight{ pHud->GetHeight()}
	, m_MaxShakeTime{0.2f}
	, m_ShakeTimer{0.f}
	, m_XShake{0}
	, m_YShake{0}
	, m_LockLocation{0.f, 0.f}
	, m_IsLocked{false}
	, m_IsUnlocking{false}
{
	const float viewHeight{ 168.f };
	float gameAreaHeight{ viewHeight + m_HudHeight };
	m_ScalingFactor.y = m_WindowSize.y / gameAreaHeight;
	m_ScalingFactor.x = m_WindowSize.x / pHud->GetWidth();
	Point2f viewDims{ m_WindowSize.x / m_ScalingFactor.x, m_WindowSize.y / m_ScalingFactor.y };
	m_VisibleArea.width = viewDims.x;
	m_VisibleArea.height = viewHeight;
}

Camera::Camera(const Point2f& location, float windowWidth, float windowHeight, HUD* pHud)
	: Camera{location.x, location.y, windowWidth, windowHeight, pHud }
{
}

void Camera::SetShake()
{
	m_IsShaking = true;
}

void Camera::Transform(float distanceFactor)
{
	// Windowsizes divided by scaling factor results in the amount of pixels the scaled window can represent
	// This is divided by 2 as well to get the center of the scaled window
	Point2f ScaledCenterOfScreen{ m_WindowSize.x / (2 * m_ScalingFactor.x), (m_WindowSize.y - m_HudHeight * m_ScalingFactor.y) / (2 * m_ScalingFactor.y)  };

	bool outOfBoundsLeft{ m_Location.x - ScaledCenterOfScreen.x < m_LevelBoundaries.left };
	bool outOfBoundsRight{ m_Location.x + ScaledCenterOfScreen.x > m_LevelBoundaries.width };
	bool outOfBoundsBottom{ m_Location.y - ScaledCenterOfScreen.y < m_LevelBoundaries.bottom };
	bool outOfBoundsTop{ m_Location.y + ScaledCenterOfScreen.y > m_LevelBoundaries.height };

	float screenCenterLeft{ ScaledCenterOfScreen.x / distanceFactor };
	float screenCenterBottom{ ScaledCenterOfScreen.y / distanceFactor };
	float screenCenterRight{ (m_LevelBoundaries.width - ScaledCenterOfScreen.x) / distanceFactor };
	float screenCenterTop{ (m_LevelBoundaries.height - ScaledCenterOfScreen.y) / distanceFactor };

	float reCenterOnX{}, reCenterOnY{};

	if (outOfBoundsLeft) reCenterOnX = screenCenterLeft;
	else if (outOfBoundsRight) reCenterOnX = screenCenterRight;
	else reCenterOnX = m_Location.x / distanceFactor;

	if (outOfBoundsBottom) reCenterOnY = screenCenterBottom;
	else if (outOfBoundsTop) reCenterOnY = screenCenterTop;
	else reCenterOnY = m_Location.y / distanceFactor; 

	glScalef(m_ScalingFactor.x, m_ScalingFactor.y, 1.f); // Scaling to fit window
	glTranslatef(screenCenterLeft, screenCenterBottom, 0.f); // Translate camera position to center of screen
	glTranslatef(0.f, m_HudHeight, 0.f); // Get view area above hud
	glTranslatef(-reCenterOnX, -reCenterOnY, 0.f);

	if (m_ShakeTimer > 0.f)
	{
		const float zoomFactor{ 1.005f }; // To mask that shaking might reveal areas that are outside the level boundaries
		glScalef(zoomFactor, zoomFactor, 0.f);
		glTranslatef(float(m_XShake), float(m_YShake), 0.f);
	}
}

void Camera::UpdateLocation(float x, float y, float elapsedSec)
{
	const float xMovementTreshold(48.f);

	if (m_Location.x - x > xMovementTreshold)
	{
		m_Location.x = x + xMovementTreshold;
	}
	else if (m_Location.x - x < 0.f)
	{
		m_Location.x = x;
	}

	const float yMovementTreshold(24.f);
	if (m_Location.y - y > yMovementTreshold)
	{
		m_Location.y = y + yMovementTreshold;
	}
	else if (m_Location.y - y < 0.f)
	{
		m_Location.y = y;
	}
}

void Camera::CalculateShake()
{
	const int maxShake{ 2 };
	m_XShake = rand() % maxShake - maxShake / 2;
	m_YShake = rand() % maxShake - maxShake / 2;
}

void Camera::Update(float elapsedSec, Kirby* pKirby)
{
	if (m_IsLocked)
	{
		MoveToLockLocation(elapsedSec);
		LockKirbyToVisibleArea(pKirby);
	}
	else if (m_IsUnlocking)
	{
		MoveBackToKirby(elapsedSec, pKirby);
	}
	else
	{
		UpdateLocation(pKirby->GetLocation().x, pKirby->GetLocation().y, elapsedSec);
	}
	CalculateVisibleArea();
	UpdateShake(elapsedSec);
}


void Camera::UpdateShake(float elapsedSec)
{
	if (m_IsShaking)
	{
		m_ShakeTimer += elapsedSec;
		m_IsShaking = false;
	}

	if (m_ShakeTimer > m_MaxShakeTime)
	{
		m_ShakeTimer = 0.f;
	}
	else if (m_ShakeTimer > 0.f)
	{
		CalculateShake();
		m_ShakeTimer += elapsedSec;
	}
}

void Camera::LockKirbyToVisibleArea(Kirby* pKirby)
{
	Rectf kirbyShape{ pKirby->GetShape() };
	Point2f lockLocation{kirbyShape.left, kirbyShape.bottom};
	if (kirbyShape.left < m_VisibleArea.left)
	{
		lockLocation.x = m_VisibleArea.left;
	}
	else if (kirbyShape.left + kirbyShape.width > m_VisibleArea.left + m_VisibleArea.width)

	{
		lockLocation.x = m_VisibleArea.left + m_VisibleArea.width - kirbyShape.width;
	}

	if (kirbyShape.bottom < m_VisibleArea.bottom)
	{
		lockLocation.y = m_VisibleArea.bottom;
	}
	else if (kirbyShape.bottom + kirbyShape.height > m_VisibleArea.bottom + m_VisibleArea.height)

	{
		lockLocation.y = m_VisibleArea.bottom + m_VisibleArea.height - kirbyShape.height;
	}

	pKirby->SetLocation(lockLocation);
}

void Camera::MoveToLockLocation(float elapsedSec)
{
	const float tolerance{ 2.f };
	const float speed{ 200.f };
	const float locationDifference{ m_LockLocation.x - m_Location.x};
	if (abs(locationDifference) > tolerance)
	{
		m_Location.x += speed * elapsedSec * (locationDifference / abs(locationDifference));
	}
}

void Camera::MoveBackToKirby(float elapsedSec, Kirby* pKirby)
{
	const float tolerance{ 2.f };
	const float speed{ 200.f };
	const float locationDifference{ pKirby->GetShape().left - m_Location.x};
	if (abs(locationDifference) > tolerance)
	{
		m_Location.x += speed * elapsedSec * (locationDifference / abs(locationDifference));
	}
	else
	{
		m_IsUnlocking = false;
	}
}

void Camera::TransformHUD()
{
	glScalef(m_ScalingFactor.x, m_ScalingFactor.y, 1.f); // Scaling to fit window
}

void Camera::UpdateBoundaries(const Rectf& levelBounds)
{
	m_LevelBoundaries = levelBounds;
}

void Camera::SetLocked(const Point2f& lockLocation)
{
	m_IsLocked = true;
	m_LockLocation = lockLocation;
	CalculateVisibleArea();
}

bool Camera::IsLocked() const
{
	return m_IsLocked;
}

void Camera::Unlock()
{
	m_IsLocked = false;
	m_IsUnlocking = true;
}

Point2f Camera::GetLocation()
{
	return m_Location;
}

Rectf Camera::GetVisibleArea()
{
	return m_VisibleArea;
}

void Camera::CalculateVisibleArea()
{
	Point2f ScaledCenterOfScreen{ m_WindowSize.x / (2 * m_ScalingFactor.x), (m_WindowSize.y - m_HudHeight * m_ScalingFactor.y) / (2 * m_ScalingFactor.y) };

	bool outOfBoundsLeft{ m_Location.x - ScaledCenterOfScreen.x < m_LevelBoundaries.left };
	bool outOfBoundsRight{ m_Location.x + ScaledCenterOfScreen.x > m_LevelBoundaries.width };
	bool outOfBoundsBottom{ m_Location.y - ScaledCenterOfScreen.y < m_LevelBoundaries.bottom };
	bool outOfBoundsTop{ m_Location.y + ScaledCenterOfScreen.y > m_LevelBoundaries.height };

	if (outOfBoundsLeft)
	{
		m_VisibleArea.left = m_LevelBoundaries.left;
	}
	else if (outOfBoundsRight)
	{
		m_VisibleArea.left = m_LevelBoundaries.width - m_VisibleArea.width;
	}
	else
	{
		m_VisibleArea.left = m_Location.x - m_VisibleArea.width / 2;
	}

	if (outOfBoundsBottom)
	{
		m_VisibleArea.bottom = m_LevelBoundaries.bottom;
	}
	else if (outOfBoundsTop)
	{
		m_VisibleArea.bottom = m_LevelBoundaries.height - m_VisibleArea.height;
	}
	else
	{
		m_VisibleArea.bottom = m_Location.y - m_VisibleArea.height / 2;
	}
}
