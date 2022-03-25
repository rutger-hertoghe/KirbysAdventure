#include "pch.h"
#include "Kirby.h"
#include "PowerUp.h"
#include "Sprite.h"
#include <iostream>
#include "Level.h"
#include "ProjectileManager.h"
#include "Star.h"
#include "Utils.h"

Kirby::Kirby()
	: m_ActionState{ ActionState::idle }
	, m_MaxHorSpeed{ 80.f }
	, m_HorAcceleration{ 250.f }
	, m_JumpSpeed{ 180.f }
	, m_MaxJumpTime{ 0.15f }
	, m_JumpTime{ 0.f }
	, m_MacroState{ MacroState::basic }
	, m_Health{ 4 }
	, m_Lives{ 5 }
	, m_MaxHealth{ 6 }
	, m_IsInvulnerable{false}
	, m_CanSpitStar{false}
{
	Initialize();
}

Kirby::~Kirby()
{
	DeleteSprites();
}

void Kirby::DeleteSprites()
{
	for (Sprite* pSprite : m_pSprites)
	{
		if (pSprite) delete pSprite;
	}
	m_pSprites.clear();
}

void Kirby::Update(float elapsedSec)
{
	UpdateSprite(elapsedSec);
	SetIsOnGround();
	ProcessInput(elapsedSec);
	UpdateState();
	ProcessMovement(elapsedSec);
	LockToLevel();
	if (m_pPowerUp) m_pPowerUp->Update(elapsedSec);
}

void Kirby::ProcessInput(float elapsedSec)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	const bool isUsingPower{ m_pPowerUp ? m_pPowerUp->IsActive() : false };
	const bool isImmobile{
		isUsingPower
		|| m_MacroState == MacroState::inhalation
		|| (m_MacroState == MacroState::ducking && m_IsOnGround)
		|| m_MacroState == MacroState::sliding };
	
	// LEFT/RIGHT MOVEMENT FUNCTIONALITY
	const bool rightKeysDown{ pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D] };
	const bool leftKeysDown{  pStates[SDL_SCANCODE_LEFT]  || pStates[SDL_SCANCODE_A] };
	if (leftKeysDown)
	{
		m_XDirection = -1.f;
		const bool canAccelerate{ m_Velocity.x > -m_MaxHorSpeed };
		if (!isImmobile) Move(elapsedSec, canAccelerate);
		else SlowDown(elapsedSec);
	}
	else if (rightKeysDown)
	{
		m_XDirection = 1.f;
		const bool canAccelerate{ m_Velocity.x < m_MaxHorSpeed };
		if (!isImmobile) Move(elapsedSec, canAccelerate);
		else SlowDown(elapsedSec);
	}
	else
	{
		SlowDown(elapsedSec);
	}

	// DOWN KEY FUNCTIONALITY
	const bool downKeysDown{ pStates[SDL_SCANCODE_DOWN] || pStates[SDL_SCANCODE_S] };
	if (downKeysDown && m_MacroState == MacroState::basic)
	{
		if (m_pPowerUp && !m_pPowerUp->IsActive())
		{
			m_MacroState = MacroState::ducking;
		}
	}
	else if (!downKeysDown && m_MacroState == MacroState::ducking)
	{
		m_MacroState = MacroState::basic;
	}

	// SPACE BAR FUNCTIONALITY
	if (pStates[SDL_SCANCODE_SPACE] && !isImmobile)
	{
		if (m_MacroState == MacroState::inflated)	Flap();
		else if (m_MacroState != MacroState::ducking && m_MacroState !=	MacroState::sliding)
			Jump(elapsedSec);
	}

	// UP KEY FUNCTIONALITY
	const bool upKeysDown{ (pStates[SDL_SCANCODE_UP] || pStates[SDL_SCANCODE_W]) };
	if (upKeysDown && !isImmobile)
	{
		if (m_MacroState != MacroState::bloated)	Flap();
		else										Jump(elapsedSec);
	}

	// R KEY FUNCTIONALITY
	if (pStates[SDL_SCANCODE_R] && m_MacroState == MacroState::basic)
	{
		if (m_pPowerUp)	m_pPowerUp->ContinuousKeyEvent(m_Shape, m_XDirection);
	}
}

void Kirby::ProcessKeyUp(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
	case SDLK_SPACE:
		m_JumpTime = m_MaxJumpTime;
		break;
	case SDLK_w:
		--m_Health;
		break;
	case SDLK_r:
		if (m_MacroState == MacroState::bloated)
		{
			m_CanSpitStar = true;
		}
		else if (m_MacroState == MacroState::inhalation)
		{
			m_MacroState = MacroState::basic;
		}
		else if (m_pPowerUp && m_MacroState == MacroState::basic)
		{
			m_pPowerUp->OnKeyUpEvent(m_Shape, m_XDirection);
		}
		break;
	}
}

void Kirby::ProcessKeyDown(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
	case SDLK_r:
		if (m_MacroState == MacroState::inflated)
		{
			m_Velocity.y = 0.f;
			m_MacroState = MacroState::basic;
		}
		else if (m_pPowerUp && m_MacroState == MacroState::basic)
		{
			m_pPowerUp->OnKeyDownEvent(m_Shape, m_XDirection);
		}
		else if (m_MacroState == MacroState::bloated)
		{
			if (m_CanSpitStar)
			{
				SetState(ActionState::spitting);
				m_MacroState = MacroState::basic;
				SpitStar();
			}
		}
		else if (!m_pPowerUp)
		{
			m_MacroState = MacroState::inhalation;
		}
		break;
	// TEST CODE
	case SDLK_e:
		if (m_MacroState == MacroState::bloated) m_MacroState = MacroState::basic;
		break;
	case SDLK_SPACE:
		if (m_MacroState == MacroState::ducking && m_IsOnGround && !m_pPowerUp->IsActive())
		{
			m_Velocity.x = 180.f * m_XDirection;
			m_MacroState = MacroState::sliding;
		}
		break;
	}
}

#pragma region MovementActions

void Kirby::Move(float elapsedSec, bool canAccelerate)
{
	if (canAccelerate) m_Velocity.x += m_XDirection * m_HorAcceleration * elapsedSec;
	else m_Velocity.x = m_XDirection * m_MaxHorSpeed;
}

void Kirby::SlowDown(float elapsedSec)
{
	if (m_Velocity.x < 5.f && m_Velocity.x > -5.f) m_Velocity.x = 0;
	else if (m_Velocity.x > 1.f) m_Velocity.x -= m_HorAcceleration * elapsedSec;
	else if (m_Velocity.x < -1.f) m_Velocity.x += m_HorAcceleration * elapsedSec;
}

void Kirby::Jump(float elapsedSec)
{
	if (m_JumpTime < m_MaxJumpTime) AccelerateJump(elapsedSec);
}

void Kirby::AccelerateJump(float elapsedSec)
{
	m_Velocity.y = m_JumpSpeed;
	m_JumpTime += elapsedSec;
}

void Kirby::Flap()
{
	m_MacroState = MacroState::inflated;
	m_Velocity.y = m_JumpSpeed;
	m_JumpTime = m_MaxJumpTime;
}
#pragma endregion

void Kirby::DiscardPower()
{
	delete m_pPowerUp;
	m_pPowerUp = nullptr;
}

void Kirby::ProcessMovement(float elapsedSec)
{
	float xVelocity{ m_Velocity.x }, yVelocity{ m_Velocity.y };
	if (m_MacroState == MacroState::inflated)
	{
		const float speedModifier{ 0.5f };
		xVelocity *= speedModifier;
		yVelocity *= speedModifier;
	}
	ApplyVelocities(elapsedSec, xVelocity, yVelocity);
	ApplyGravity(elapsedSec);
	HandleCollisions();
}

void Kirby::UpdateState()
{
	const float yVelocityTreshold{ 100.f };
	const bool isMoving{ abs(m_Velocity.x) >= 1.f };
	const bool isIdle{ !isMoving };
	const bool isGoingUp{ m_Velocity.y > 1.f };
	const bool isGoingDown{ m_Velocity.y < -1.f };
	const bool isJumping{ isGoingUp && !(m_ActionState == ActionState::inflating || m_ActionState == ActionState::inflated)};
	const bool isMidAir{ abs(m_Velocity.y) < yVelocityTreshold && !m_IsOnGround };
	const bool isFalling{!m_IsOnGround && m_ActionState != ActionState::jumping};
	const bool isLoopDone{ m_HasLooped };
	const bool isUsingPower{ m_pPowerUp?m_pPowerUp->IsActive() : false };

	const bool isInflated{ m_MacroState == MacroState::inflated };
	const bool isDucking{ m_MacroState == MacroState::ducking };
	const bool isInhaling{ m_MacroState == MacroState::inhalation };
	const bool isBloated{ m_MacroState == MacroState::bloated };
	const bool isSliding{ m_MacroState == MacroState::sliding };

	switch (m_ActionState)
	{
	// BASIC SET
	case ActionState::idle:
		if (isInflated)			SetState(ActionState::inflating);
		else if (isUsingPower)	SetPowerState();
		else if (isInhaling)	SetState(ActionState::start_inhaling);
		else if (isJumping)		SetState(ActionState::jumping);
		else if (isFalling)		SetState(ActionState::falling);
		else if (isDucking)		SetState(ActionState::ducking);
		else if (isMoving)		SetState(ActionState::walking);
		break;
	case ActionState::walking:
		if (isInflated)			SetState(ActionState::inflating);
		else if (isUsingPower)	SetPowerState();
		else if (isInhaling)	SetState(ActionState::start_inhaling);
		else if (isJumping)		SetState(ActionState::jumping);
		else if (isFalling)		SetState(ActionState::falling);
		else if (isDucking)		SetState(ActionState::ducking);
		else if (isIdle)		SetState(ActionState::idle);
		break;
	case ActionState::jumping:
		if (isInflated)			SetState(ActionState::inflating);
		else if (isUsingPower)	SetPowerState();
		else if (m_IsOnGround)	SetState(ActionState::idle);		
		else if (isMidAir)		SetState(ActionState::flipping);
		break;
	case ActionState::flipping:
		if (isInflated)			SetState(ActionState::inflating);
		else if (isUsingPower)  SetPowerState();
		else if (m_IsOnGround)	SetState(ActionState::idle);
		else if (isLoopDone)	SetState(ActionState::falling);
		break;
	case ActionState::falling:
		if (isInflated)			SetState(ActionState::inflating);
		else if (isUsingPower)	SetPowerState();
		else if (m_IsOnGround)	SetState(ActionState::idle);
		break;
	case ActionState::ducking:
		if (isSliding)			SetState(ActionState::sliding);
		else if (!isDucking)	SetState(ActionState::idle);
		break;
	case ActionState::sliding:
		if (!isMoving)
		{
			SetState(ActionState::ducking);
			m_MacroState = MacroState::ducking;
		}
	// INFLATING SET
	case ActionState::inflating:
		if (isLoopDone)	SetState(ActionState::inflated);
		break;
	case ActionState::inflated:
		if		(!isInflated)	SetState(ActionState::deflating);
		else if (isGoingUp)		SetState(ActionState::flapping);
		break;
	case ActionState::flapping:
		if		(!isInflated)	SetState(ActionState::deflating);
		else if (isGoingDown)	SetState(ActionState::inflated);
		break;
	case ActionState::deflating:
		if (isLoopDone)	SetState(ActionState::idle);
		break;
	// INHALING/BlOAT SET
	case ActionState::start_inhaling:
		if (isLoopDone)	SetState(ActionState::inhaling);
		break;
	case ActionState::inhaling:
		if (isBloated)			SetState(ActionState::bloat);
		else if (!isInhaling)	SetState(ActionState::exhaling);
		break;
	case ActionState::bloat:
		if (isLoopDone) SetState(ActionState::bloated_idle);
		break;
	case ActionState::exhaling:
		if (isLoopDone) SetState(ActionState::idle);
		break;
	case ActionState::bloated_idle:
		if (!isBloated) SetState(ActionState::swallowing);
		else if (isMoving || isJumping || isFalling) SetState(ActionState::bloated_walking);
		break;
	case ActionState::bloated_walking:
		if (!isBloated) SetState(ActionState::swallowing);
		else if (isIdle && !isJumping && !isFalling) SetState(ActionState::bloated_idle);
		break;
	case ActionState::swallowing:
		if (isLoopDone) SetState(ActionState::idle);
		break;
	case ActionState::spitting:
		if (isLoopDone) SetState(ActionState::idle);
		break;
	// POWER SET
	case ActionState::power_start:
		if (!isUsingPower && m_pPowerUp->HasEnd()) SetState(ActionState::power_end);
		else if (!isUsingPower) SetState(ActionState::idle);
		else if (isLoopDone && m_pPowerUp->HasContinuous()) SetState(ActionState::power_continuous);
		break;
	case ActionState::power_continuous:
		if (!isUsingPower && m_pPowerUp->HasEnd()) SetState(ActionState::power_end);
		else if (!isUsingPower) SetState(ActionState::idle);
		break;
	case ActionState::power_end:
		if (isLoopDone) SetState(ActionState::idle);
		break;
	}
}

void Kirby::Initialize()
{
	InitializeSprites();
	m_pCurrentSprite = GetSpritePtr("kirby_idle");
	Point2f kirbySize{ m_pCurrentSprite->GetFrameDimensions() };
	m_Shape.width = kirbySize.x;
	m_Shape.height = kirbySize.y;
}

void Kirby::InitializeSprites()
{
	int nrFrames{ 1 };
	float animationSpeed{ 0.f };
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_jumping.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_falling.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_inhaling.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_bloated_idle.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_ability_jumping.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_ability_falling.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_ducking.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_sliding.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_ability_ducking.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_ability_sliding.png" });
	animationSpeed = 0.15f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_ability_spark_start.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_hurt.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_inflated_hurt.png" });

	nrFrames = 2;
	animationSpeed = 0.05f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_ability_fire_continuous.png" });
	animationSpeed = 0.15f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_start_inhaling.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_exhaling.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_flapping.png", });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_ability_flapping.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_ability_spark_continuous.png" });
	animationSpeed = 0.75f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_inflated.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_ability_inflated.png" });

	nrFrames = 3;
	animationSpeed = 0.3f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_swallowing.png" });

	nrFrames = 4;
	animationSpeed = 0.2f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_flipping.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_ability_flipping.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_bloat.png" });
	animationSpeed = 0.4f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_inflating.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_deflating.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_ability_inflating.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_ability_deflating.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_spitting.png" });
	animationSpeed = 0.7f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_walking.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_ability_walking.png" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_bloated_walking.png" });

	nrFrames = 16;
	animationSpeed = 1.6f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_idle.png", 2 });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "resources/sprites/kirby_ability_idle.png", 2 });
}


#pragma region Utility
Sprite* Kirby::GetSpritePtr(const std::string& spriteName) const
{
	for (Sprite* pSprite : m_pSprites)
	{
		if (pSprite->GetName() == spriteName)
		{
			return pSprite;
		}
	}
	return nullptr;
}

void Kirby::SetIsOnGround()
{
	Actor::SetIsOnGround();
	if (m_IsOnGround)
	{
		m_JumpTime = 0.f;
	}
}

void Kirby::SpitStar()
{
	// Copy shape first to manipulate copied shape properties underneath, then offset to match sprite center and not spawn inside Kirby
	Rectf spawnRect{ m_Shape };																
	const float yOffset{ (m_pCurrentSprite->GetFrameDimensions().y - m_Shape.height) / 2 }; 
	spawnRect.left += m_XDirection * m_Shape.width;	
	spawnRect.bottom += yOffset;
	m_pProjectileManager->Add(new Star{ spawnRect, m_XDirection });

	// Spitting out the star should remove the power
	DiscardPower();
}

Rectf Kirby::GetInhalationZone() const
{
	// Return offscreen rectangle so no enemies get influenced
	if (m_MacroState != MacroState::inhalation)
	{
		return Rectf{ -10.f, -10.f, 0.f, 0.f };
	}

	Point2f inhalationZoneDims{ 100.f, 200.f };
	Rectf inhalationZone{ 0.f, m_Shape.bottom, inhalationZoneDims.x, inhalationZoneDims.y };
	inhalationZone.left = (m_XDirection > 0.f) ? m_Shape.left + m_Shape.width : m_Shape.left - inhalationZoneDims.x;
	inhalationZone.bottom = (m_Shape.bottom + m_pCurrentSprite->GetFrameDimensions().y / 2) - (inhalationZoneDims.y / 2);
	return inhalationZone;
	
}

void Kirby::SetBloated()
{
	m_MacroState = MacroState::bloated;
}

void Kirby::SetState(const ActionState& state)
{
	m_ActionState = state;
	std::string spriteName{ GetSpriteNameFromState(state) };
	m_pCurrentSprite = GetSpritePtr(spriteName);
	m_AccumulatedTime = 0;
	m_CurrentFrame = 0;
	m_HasLooped = 0;
	m_LoopProgressTime = 0;
}

void Kirby::LockToLevel()
{
	float& kirbyLeft{ m_Shape.left };
	float& kirbyBottom{ m_Shape.bottom };
	const float kirbyRight{ kirbyLeft + m_Shape.width };
	const float kirbyTop{ kirbyBottom + m_Shape.height };
	const float yOffsetScalar{ 1.5f };
	const float topBorder{ yOffsetScalar * m_Shape.height };
	Rectf boundaries{ m_pCurrentLevel->GetBoundaries() };

	if (kirbyBottom > boundaries.height - topBorder)
	{
		kirbyBottom = boundaries.height - topBorder;
		m_Velocity.y = 0.f;
	}
	if (kirbyLeft < boundaries.left) kirbyLeft = boundaries.left;
	else if (kirbyRight > boundaries.width) kirbyLeft = boundaries.width - m_Shape.width;

	if (kirbyBottom < -50.f)
	{
		KillKirby();
	}
}

std::string Kirby::GetSpriteNameFromState(const ActionState& state) const
{
	std::string spriteName{"kirby_"};

	if (m_pPowerUp)
	{
		spriteName.append("ability_");
		if (m_pPowerUp->HasCompleteSpriteSet())
		{
			spriteName.append(m_pPowerUp->GetPowerSuffix());
		}
	}
	switch (state)
	{
	case ActionState::idle:
		spriteName.append("idle");
		break;
	case ActionState::walking:
		spriteName.append("walking");
		break;
	case ActionState::jumping:
		spriteName.append("jumping");
		break;
	case ActionState::flipping:
		spriteName.append("flipping");
		break;
	case ActionState::falling:
		spriteName.append("falling");
		break;
	case ActionState::ducking:
		spriteName.append("ducking");
		break;
	case ActionState::sliding:
		spriteName.append("sliding");
		break;
	case ActionState::inflating:
		spriteName.append("inflating");
		break;
	case ActionState::inflated:
		spriteName.append("inflated");
		break;
	case ActionState::flapping:
		spriteName.append("flapping");
		break;
	case ActionState::deflating:
		spriteName.append("deflating");
		break;
	// STATES ONLY ACCESSIBLE WHEN NO POWER
	case ActionState::swallowing:
		spriteName = "kirby_swallowing";
		break;
	case ActionState::start_inhaling:
		spriteName = "kirby_start_inhaling";
		break;
	case ActionState::inhaling:
		spriteName = "kirby_inhaling";
		break;
	case ActionState::exhaling:
		spriteName = "kirby_exhaling";
		break;
	case ActionState::bloated_idle:
		spriteName = "kirby_bloated_idle";
		break;
	case ActionState::bloated_walking:
		spriteName = "kirby_bloated_walking";
		break;
	case ActionState::bloat:
		spriteName = "kirby_bloat";
		break;
	case ActionState::spitting:
		spriteName = "kirby_spitting";
		break;
	// POWER STATES
	case ActionState::power_start:
		if (m_pPowerUp && !(m_pPowerUp->HasCompleteSpriteSet()))
		{
			spriteName.append(m_pPowerUp->GetPowerSuffix());
		}
		spriteName.append("start");
		break;
	case ActionState::power_continuous:
		if (m_pPowerUp && !(m_pPowerUp->HasCompleteSpriteSet()))
		{
			spriteName.append(m_pPowerUp->GetPowerSuffix());
		}
		spriteName.append("continuous");
		break;
	case ActionState::power_end:
		if (m_pPowerUp && !(m_pPowerUp->HasCompleteSpriteSet()))
		{
			spriteName.append(m_pPowerUp->GetPowerSuffix());
		}
		spriteName.append("end");
		break;
	}
	return spriteName;
}
#pragma endregion

int Kirby::GetHealth() const
{
	return m_Health;
}

int Kirby::GetLives() const
{
	return m_Lives;
}

void Kirby::SetProjectileManager(ProjectileManager* pProjectileMgr)
{
	m_pProjectileManager = pProjectileMgr;
}

void Kirby::Hit()
{
}

void Kirby::DecrementHealth()
{
	if (m_IsInvulnerable) return;

	--m_Health;
	if (m_Health <= 0)
	{
		KillKirby();
	}
	m_IsInvulnerable = true;
}

void Kirby::KillKirby()
{
	--m_Lives;
	SetLocation(m_pCurrentLevel->GetStartLocation());
	m_Velocity.y = 0;
}

void Kirby::Draw() const
{
	if (m_MacroState == MacroState::inhalation)
	{
		Color4f transparentCyan{ 0.f, 0.8f, 0.8f, 0.3f };
		utils::SetColor(transparentCyan);
		utils::FillRect(GetInhalationZone());
	}
	GameObject::Draw();
}

bool Kirby::IsInhaling() const
{
	if (m_MacroState == MacroState::inhalation)
	{
		return true;
	}
	return false;
}

void Kirby::EnforceState()
{
	SetState(m_ActionState);
}

void Kirby::SetPowerState()
{
	if (m_pPowerUp->HasStart())
	{
		SetState(ActionState::power_start);
	}
	else if (m_pPowerUp->HasContinuous())
	{
		SetState(ActionState::power_continuous);
	}
	else if (m_pPowerUp->HasEnd())
	{
		SetState(ActionState::power_end);
	}
}

