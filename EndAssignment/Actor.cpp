#include "pch.h"
#include "Actor.h"
#include "PowerUp.h"
#include "Sprite.h"
#include "ProjectileManager.h"
#include "utils.h"

const float Actor::m_Gravity{ -500.f };

Actor::Actor()
	: GameObject{}
	, m_pPowerUp{ nullptr }
	, m_pCurrentLevel{ nullptr }
	, m_BaseVelocity{0.f, 0.f}
	, m_IsOnGround{false}
	, m_IsInhalable{ false }
	, m_IsBeingInhaled{ false }
{
}

Actor::~Actor()
{
	if (m_pPowerUp)
	{
		delete m_pPowerUp;
		m_pPowerUp = nullptr;
	}
}

void Actor::TransferPowerUp(Actor* newOwner)
{
	if (newOwner->m_pPowerUp)
	{
		delete newOwner->m_pPowerUp;
	}
	newOwner->m_pPowerUp = m_pPowerUp;
	m_pPowerUp = nullptr;
}

void Actor::SetPowerUp(PowerUp* powerUp, ProjectileManager* pProjectileMgr)
{
	if (m_pPowerUp)
	{
		delete m_pPowerUp;
	}
	m_pPowerUp = powerUp;
	m_pPowerUp->SetProjectileManager(pProjectileMgr);
}

bool Actor::HasPower()
{
	if (m_pPowerUp) return true;
	return false;
}

void Actor::SetLocation(const Point2f& location)
{
	m_Shape.left = location.x;
	m_Shape.bottom = location.y;
}

Point2f Actor::GetLocation() const
{
	return Point2f(m_Shape.left, m_Shape.bottom);
}

PowerUp* Actor::GetPowerUp() const
{
	return m_pPowerUp;
}

void Actor::SetCurrentLevel(Level* level)
{
	m_pCurrentLevel = level;
}

void Actor::SetIsOnGround()
{
	if (m_pCurrentLevel->IsOnGround(m_Shape))
	{
		m_IsOnGround = true;
	}
	else m_IsOnGround = false;
}

void Actor::SetBaseVelocity(float xVelocity, float yVelocity)
{
	m_Velocity.x = xVelocity;
	m_Velocity.y = yVelocity;
	m_BaseVelocity = m_Velocity;
}

void Actor::SetBaseVelocity(const Vector2f& velocity)
{
	SetBaseVelocity(velocity.x, velocity.y);
}

void Actor::SetBaseVelocity(const Point2f& velocity)
{
	SetBaseVelocity(velocity.x, velocity.y);
}

void Actor::ApplyVelocities(float elapsedSec, float xVelocity, float yVelocity)
{
	m_Shape.left += xVelocity * elapsedSec;
	m_Shape.bottom += yVelocity * elapsedSec;
}

void Actor::ApplyGravity(float elapsedSec)
{
	m_Velocity.y += m_Gravity * elapsedSec;
}

void Actor::HandleCollisions()
{
	m_pCurrentLevel->HandleCollision(m_Shape, m_Velocity);
}

void Actor::SetInhalationVelocities(const Rectf& kirbyRect)
{
	if (!m_IsBeingInhaled) return; // Stop further execution if actor is not being inhaled
	
	const float xDistanceToKirby{ (m_Shape.left + m_Shape.width / 2) - (kirbyRect.left + kirbyRect.width / 2 ) };
	const float yDistanceToKirby{ (m_Shape.bottom + m_Shape.height / 2) - (kirbyRect.bottom + kirbyRect.height / 2 ) };
	const float speedMagnifier{ 5000 };
	m_Velocity.x = (-speedMagnifier / xDistanceToKirby);
	m_Velocity.y = -yDistanceToKirby;
}

bool Actor::IsInhalable()
{
	return m_IsInhalable;
}

bool Actor::IsBeingInhaled()
{
	return m_IsBeingInhaled;
}

void Actor::ToggleBeingInhaled(const Rectf& inhalationZone)
{
	Point2f actorCenter{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2 };
	if (utils::IsPointInRect(actorCenter, inhalationZone))
	{
		m_IsBeingInhaled = true;
	}
	else
	{
		m_IsBeingInhaled = false;
	}
}

