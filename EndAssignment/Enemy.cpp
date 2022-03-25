#include "pch.h"
#include "Enemy.h"
#include "utils.h"

Enemy::Enemy(const Point2f& location)
	: Actor{}
	, m_IsActive{true}
{
	m_Shape.left = location.x;
	m_Shape.bottom = location.y;
}

Enemy::~Enemy()
{
}

void Enemy::Update(float elapsedSec)
{
	ApplyVelocities(elapsedSec, m_Velocity.x, m_Velocity.y);
	if (m_IsBeingInhaled) return; // If enemy is being inhaled, code below can not be executed
	
	UpdateSprite(elapsedSec);
	CorrectVelocities();
	ApplyGravity(elapsedSec);
	HandleCollisions();
}

bool Enemy::CheckCollision(const Rectf& kirbyRect)
{
	Point2f enemyCenter{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2 };
	if (utils::IsPointInRect(enemyCenter, kirbyRect))
	{
		return true;
	}
	return false;
}

void Enemy::CorrectVelocities()
{
	if (m_Velocity.x != m_BaseVelocity.x)
	{
		m_Velocity.x = m_BaseVelocity.x;
	}
}
