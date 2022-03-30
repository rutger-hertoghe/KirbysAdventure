#include "pch.h"
#include "Enemy.h"
#include "utils.h"

Enemy::Enemy(const Point2f& location)
	: Actor{}
	, m_IsActive{false}
	, m_HasBeenOffScreen{true}
{
	m_StartLocation = location;
	m_Shape.left = location.x;
	m_Shape.bottom = location.y;
}

void Enemy::Update(float elapsedSec)
{
	ApplyVelocities(elapsedSec, m_XDirection * m_Velocity.x, m_Velocity.y);
	if (m_IsBeingInhaled) return; // If enemy is being inhaled, code below can not be executed
	
	UpdateSprite(elapsedSec);
	CorrectVelocities();
	ApplyGravity(elapsedSec);
	HandleCollisions();
}

bool Enemy::CheckCollision(const Rectf& kirbyRect)
{
	if (!m_IsActive) return false;

	Point2f enemyCenter{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2 };
	if (utils::IsPointInRect(enemyCenter, kirbyRect))
	{
		return true;
	}
	return false;
}

bool Enemy::IsActive() const
{
	return m_IsActive;
}

void Enemy::Reset()
{
	m_IsActive = false;
	m_HasBeenOffScreen = false;
	m_Shape.left = m_StartLocation.x;
	m_Shape.bottom = m_StartLocation.y;
}

void Enemy::SetActivity(bool active)
{
	m_IsActive = active;
}

bool Enemy::HasBeenOffScreen()
{
	return m_HasBeenOffScreen;
}

void Enemy::SetOffScreen(bool offscreen, float direction)
{
	m_XDirection = direction;
	m_HasBeenOffScreen = true;
}

void Enemy::CorrectVelocities()
{
	if (m_Velocity.x != m_BaseVelocity.x)
	{
		m_Velocity.x = m_BaseVelocity.x;
	}
}
