#include "pch.h"
#include "Camera.h"
#include "Level.h"
#include "HUD.h"
#include <iostream>

Camera::Camera(float startLocationX, float startLocationY, float windowWidth, float windowHeight, const Rectf& levelBounds, const HUD& hud)
	: m_Location{ startLocationX, startLocationY }
	, m_WindowSize{ windowWidth, windowHeight}
	, m_HudHeight{hud.GetHeight()}
	, m_LevelBoundaries{ levelBounds }
{
	float gameAreaHeight{ m_LevelBoundaries.height + m_HudHeight };
	m_ScalingFactor.y = m_WindowSize.y / gameAreaHeight;
	m_ScalingFactor.x = m_WindowSize.x / hud.GetWidth();
}

Camera::Camera(const Point2f& location, float windowWidth, float windowHeight, const Rectf& levelBounds, const HUD& hud)
	: Camera{location.x, location.y, windowWidth, windowHeight, levelBounds, hud}
{
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

	if (outOfBoundsLeft)
	{
		reCenterOnX = screenCenterLeft;
	}
	else if (outOfBoundsRight)
	{
		reCenterOnX = screenCenterRight;
	}
	else
	{
		reCenterOnX = m_Location.x / distanceFactor;
	}

	if (outOfBoundsBottom)
	{
		reCenterOnY = screenCenterBottom;
	}
	else if (outOfBoundsTop)
	{
		reCenterOnY = screenCenterTop;
	}
	else
	{
		reCenterOnY = m_Location.y / distanceFactor;
	}

	glScalef(m_ScalingFactor.x, m_ScalingFactor.y, 1.f); // Scaling to fit window
	glTranslatef(screenCenterLeft, screenCenterBottom, 0.f); // Translate camera position to center of screen
	glTranslatef(0.f, m_HudHeight, 0.f); // Get view area above hud
	glTranslatef(-reCenterOnX, -reCenterOnY, 0.f);
}

void Camera::Update(float x, float y, float elapsedSec)
{
	const float movementTreshold(48.f);

	if (m_Location.x - x > movementTreshold)
	{
		m_Location.x = x + movementTreshold;
	}
	else if (m_Location.x - x < 0.f)
	{
		m_Location.x = x;
	}

	m_Location.y = y;
}

void Camera::Update(const Point2f& position, float elapsedSec)
{
	Update(position.x, position.y, elapsedSec);
}

void Camera::TransformHUD()
{
	glScalef(m_ScalingFactor.x, m_ScalingFactor.y, 1.f); // Scaling to fit window
}

void Camera::UpdateBoundaries(const Rectf& levelBounds)
{
	m_LevelBoundaries = levelBounds;
}

Point2f Camera::GetViewDimensions()
{
	return Point2f{ m_WindowSize.x / m_ScalingFactor.x, m_WindowSize.y / m_ScalingFactor.y };
}

Point2f Camera::GetLocation()
{
	return m_Location;
}
