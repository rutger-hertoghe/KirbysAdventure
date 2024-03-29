#include "pch.h"
#include "Actor.h"
#include "PowerUp.h"
#include "Sprite.h"
#include "ProjectileManager.h"
#include "utils.h"
#include "LevelManager.h"


const float Actor::m_Gravity{ -500.f };

Actor::Actor(LevelManager* pLevelManager, ProjectileManager* pProjectileManager)
	: GameObject{}
	, m_pProjectileManager{pProjectileManager}
	, m_pLevelManager{pLevelManager}
	, m_pPowerUp{ nullptr }
	, m_BaseVelocity{0.f, 0.f}
	, m_IsOnGround{false}
	, m_IsInhalable{ true }
	, m_IsBeingInhaled{ false }
	, m_ArbitraryTimer{0.f}
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
	m_pPowerUp->SetInactive();
	if (newOwner->m_pPowerUp)
	{
		delete newOwner->m_pPowerUp;
	}
	newOwner->m_pPowerUp = m_pPowerUp;
	newOwner->m_pPowerUp->SetOwner(newOwner);
	m_pPowerUp = nullptr;
}

void Actor::SetPowerUp(PowerUp* powerUp)
{
	if (m_pPowerUp)
	{
		delete m_pPowerUp;
	}
	m_pPowerUp = powerUp;
}

bool Actor::HasPowerUp() const
{
	if (m_pPowerUp) return true;
	return false;
}

void Actor::DeletePowerUp()
{
	delete m_pPowerUp;
	m_pPowerUp = nullptr;
}


void Actor::AddVelocity(Vector2f velocity)
{
	m_Velocity += velocity;
}

void Actor::AddVelocity(Point2f velocity)
{
	m_Velocity.x += velocity.x;
	m_Velocity.y += velocity.y;
}

void Actor::AddVelocity(float x, float y)
{
	m_Velocity.x += x;
	m_Velocity.y += y;
}

void Actor::SetVelocity(Vector2f velocity)
{
	m_Velocity = velocity;
}

void Actor::SetVelocity(Point2f velocity)
{
	m_Velocity.x = velocity.x;
	m_Velocity.y = velocity.y;
}

void Actor::SetVelocity(float x, float y)
{
	m_Velocity.x = x;
	m_Velocity.y = y;
}

Vector2f Actor::GetVelocity() const
{
	return m_Velocity;
}

PowerUp* Actor::GetPowerUp() const
{
	return m_pPowerUp;
}

void Actor::SetIsOnGround()
{
	if (m_pLevelManager->GetCurrentLevel()->IsOnGround(m_Shape))
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

void Actor::HandleLevelCollisions()
{
	m_pLevelManager->GetCurrentLevel()->HandleCollision(m_Shape, m_Velocity);
}

void Actor::ResetArbitraryTimer()
{
	m_ArbitraryTimer = 0.f;
}

void Actor::SetInhalationVelocities(const Rectf& kirbyRect)
{
	if (!m_IsBeingInhaled) return; // Stop further execution if actor is not being inhaled
	
	const float xDistanceToKirby{ (m_Shape.left + m_Shape.width / 2) - (kirbyRect.left + kirbyRect.width / 2 ) };
	const float yDistanceToKirby{ (m_Shape.bottom + m_Shape.height / 2) - (kirbyRect.bottom + kirbyRect.height / 2 ) };
	const float xSpeedMagnifier{ 5000.f };
	const float ySpeedMagnifier{ -10.f };
	const float equalityTreshold{ 5.f };
	// Multiplication by -m_XDirection because ?
	m_Velocity.x = (-m_XDirection * xSpeedMagnifier / xDistanceToKirby);  
	m_Velocity.y = (abs(yDistanceToKirby) > equalityTreshold) ? ySpeedMagnifier * yDistanceToKirby : 0.f;
}

bool Actor::IsInhalable() const
{
	return m_IsInhalable;
}

bool Actor::IsBeingInhaled() const
{
	return m_IsBeingInhaled;
}

bool Actor::IsOnGround() const
{
	return m_IsOnGround;
}

void Actor::ToggleBeingInhaled(const Rectf& inhalationZone)
{
	Point2f actorCenter{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2 };
	if (utils::IsPointInRect(actorCenter, inhalationZone))
	{
		m_IsBeingInhaled = true;
	}
}

ProjectileManager* Actor::GetProjectileManager() const
{
	return m_pProjectileManager;
}

LevelManager* Actor::GetLevelManager() const
{
	return m_pLevelManager;
}

void Actor::Flicker(float timer, std::string& spriteName)
{
	int splitIndex{ int(spriteName.find_last_of('_')) };
	if (splitIndex == -1)
	{
		spriteName.append("_alt");
		m_pCurrentSprite = GetSpritePtr(spriteName);
	}
	else if (spriteName.substr(splitIndex) != "_alt")
	{
		spriteName.append("_alt");
		m_pCurrentSprite = GetSpritePtr(spriteName);
	}
	else if (int(timer * 10) % 2 == 0 && spriteName.substr(splitIndex) == "_alt")
	{
		spriteName = spriteName.substr(0, splitIndex);
		m_pCurrentSprite = GetSpritePtr(spriteName);
	}
}

void Actor::CreateAltSprites()
{
	int originalLength{ int(m_pSprites.size()) };
	for (int idx{}; idx < originalLength; idx++)
	{
		Sprite* pSprite{ m_pSprites[idx] };
		m_pSprites.push_back(new Sprite{ pSprite->GetNrOfFrames(), pSprite->GetLoopTime(), pSprite->GetName(), pSprite->GetTextureRows(), true });
	}
}

bool Actor::ChangeDirectionOnBump()
{
	if (m_pLevelManager->GetCurrentLevel()->IsAgainstWall(m_Shape, m_XDirection))
	{
		ChangeDirection();
		return true;
	}
	return false;
}