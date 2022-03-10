#include "pch.h"
#include "Camera.h"
#include "Level.h"
#include "HUD.h"
#include <iostream>

Camera::Camera(float startLocationX, float startLocationY, float windowWidth, float windowHeight, const Level& level, const HUD& hud)
	: m_Location{ startLocationX, startLocationY }
	, m_WindowSize{ windowWidth, windowHeight}
	, m_HudHeight{hud.GetHeight()}
	, m_LevelBoundaries{ level.GetBoundaries() }
{
	float gameAreaHeight{ m_LevelBoundaries.height + m_HudHeight };
	m_ScalingFactor.y = m_WindowSize.y / gameAreaHeight;
	m_ScalingFactor.x = m_WindowSize.x / hud.GetWidth();
}

Camera::Camera(const Point2f& location, float windowWidth, float windowHeight, const Level& level, const HUD& hud)
	: Camera{location.x, location.y, windowWidth, windowHeight, level, hud}
{
}

void Camera::Transform()
{
	// Windowsizes divided by scaling factor results in the amount of pixels the scaled window can represent
	// This is divided by 2 as well to get the center of the scaled window
	Point2f ScaledCenterOfScreen{ m_WindowSize.x / (2 * m_ScalingFactor.x), (m_WindowSize.y - m_HudHeight * m_ScalingFactor.y) / (2 * m_ScalingFactor.y)  };

	bool outOfBoundsLeft{ m_Location.x - ScaledCenterOfScreen.x < m_LevelBoundaries.left };
	bool outOfBoundsRight{ m_Location.x + ScaledCenterOfScreen.x > m_LevelBoundaries.width };
	bool outOfBoundsBottom{ m_Location.y - ScaledCenterOfScreen.y < m_LevelBoundaries.bottom };
	bool outOfBoundsTop{ m_Location.y + ScaledCenterOfScreen.y > m_LevelBoundaries.height };

	float screenCenterLeft{ ScaledCenterOfScreen.x };
	float screenCenterBottom{ ScaledCenterOfScreen.y };
	float screenCenterRight{ m_LevelBoundaries.width - ScaledCenterOfScreen.x };
	float screenCenterTop{ m_LevelBoundaries.height - ScaledCenterOfScreen.y};

	float reCenterOnX{}, reCenterOnY{};

	if (outOfBoundsLeft) reCenterOnX = screenCenterLeft;
	else if (outOfBoundsRight) reCenterOnX = screenCenterRight;
	else reCenterOnX = m_Location.x;

	if (outOfBoundsBottom) reCenterOnY = screenCenterBottom;
	else if (outOfBoundsTop) reCenterOnY = screenCenterTop;
	else reCenterOnY = m_Location.y;

	glScalef(m_ScalingFactor.x, m_ScalingFactor.y, 1.f); // Scaling to fit window
	glTranslatef(ScaledCenterOfScreen.x, ScaledCenterOfScreen.y, 0.f); // Translate camera position to center of screen
	glTranslatef(0.f, m_HudHeight, 0.f); // Get view area above hud
	glTranslatef(-reCenterOnX, -reCenterOnY, 0.f);
}

void Camera::Update(float x, float y)
{
	m_Location.x = x;
	m_Location.y = y;
}

void Camera::TransformHUD()
{
	glScalef(m_ScalingFactor.x, m_ScalingFactor.y, 1.f); // Scaling to fit window
}

void Camera::UpdateBoundaries(const Level& level)
{
	m_LevelBoundaries = level.GetBoundaries();
}