#include "pch.h"
#include "Enemy.h"
#include "utils.h"
#include "Sprite.h"
#include "PowerUp.h"
#include "Kirby.h"

Enemy::Enemy(const Point2f& location)
	: Actor{}
	, m_StartLocation{location}
	, m_ActionState(ActionState::moving)
	, m_HasBeenOffScreen{false}
	, m_CanJump{false}
{
	SetLocation(m_StartLocation); // Set location separatly 
}

Enemy::~Enemy()
{
	DeleteSprites();
}

bool Enemy::IsActive() const
{
	return m_ActionState == ActionState::dormant ? false : true;
}

void Enemy::Reset()
{
	m_IsBeingInhaled = false;
	m_ActionState = ActionState::dormant;
	m_HasBeenOffScreen = false;
	m_Shape.left = m_StartLocation.x;
	m_Shape.bottom = m_StartLocation.y;
	m_Velocity = m_BaseVelocity;

	SetInitialSprite();

	if (HasPowerUp()) return; // Code below only gets executed if the enemy had a powerup & lost it
	InitializePowerUp();
}

void Enemy::SetActivity(bool active)
{
	if (active)
	{
		m_ActionState = ActionState::moving;
	}
	else
	{
		m_ActionState = ActionState::dormant;
	}
}

bool Enemy::HasBeenOffScreen() const
{
	return m_HasBeenOffScreen;
}

void Enemy::SetOffScreen(bool offscreen, float direction)
{
	m_XDirection = direction;
	m_HasBeenOffScreen = true;
}

void Enemy::InitializePowerUp()
{
	// Nothing here because by default enemies should not have a powerup
}

void Enemy::CorrectVelocities()
{
	if (m_Velocity.x != m_BaseVelocity.x)
	{
		m_Velocity.x = m_BaseVelocity.x;
	}
}

void Enemy::DeleteSprites()
{
	for (Sprite*& pSprite : m_pSprites)
	{
		delete pSprite;
		pSprite = nullptr;
	}
	m_pCurrentSprite = nullptr;
}

void Enemy::StandardUpdateSequence(float elapsedSec)
{
	ApplyVelocities(elapsedSec, m_XDirection * m_Velocity.x, m_Velocity.y);
	if (m_IsBeingInhaled) return; // If enemy is being inhaled, code below can not be executed

	UpdateSprite(elapsedSec);
	// CorrectVelocities();
	ApplyGravity(elapsedSec);
	HandleLevelCollisions();
	SetIsOnGround();

	if (m_Velocity.x == 0.f)
	{
		m_XDirection *= -1.f;
		m_Velocity.x = m_BaseVelocity.x;
	}
}

void Enemy::Jump(float jumpStrength)
{
	const bool jumpDecider{ rand() % 2 == 1 };
	if (m_CanJump == true && m_IsOnGround == false)
	{
		jumpDecider ? (m_Velocity.y += jumpStrength) : (m_Velocity.y += 0);
		m_CanJump = false;
	}
	else if (m_CanJump == false && m_IsOnGround == true)
	{
		m_CanJump = true;
	}
}

void Enemy::DoAbilityCheck(Kirby* pKirby)
{
}

float Enemy::GetSquaredDistanceToActor(Actor* actor)
{
	const Rectf enemyShape{ GetShape() };
	const Rectf actorShape{ actor->GetShape() };
	const Point2f enemyCenter{ enemyShape.left + enemyShape.width / 2, enemyShape.bottom + enemyShape.height / 2 };
	const Point2f actorCenter{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2 };

	float squaredDist{ powf(actorCenter.x - enemyCenter.x, 2) + powf(actorCenter.y - enemyCenter.y, 2) };
	return squaredDist;
}