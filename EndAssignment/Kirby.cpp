#include "pch.h"
#include "Kirby.h"
#include "PowerUp.h"
#include "Sprite.h"
#include <iostream>
#include "Level.h"
#include "ProjectileManager.h"
#include "Star.h"
#include "Puff.h"
#include "KirbyStateHandler.h"
#include "LevelManager.h"
#include "SoundEffect.h"
#include "Door.h"
#include "StonePower.h"
#include "PowerStar.h"
#include "ObjectManager.h"
#include "Camera.h"

// TODO: Fix bug where power is also triggered upon exhaling
// TODO: Fix bug where spit star doesn't happen fast enough after button press/is sometimes ignored

Kirby::Kirby()
	: m_ActionState{ ActionState::idle }
	, m_MaxHorSpeed{ 80.f }
	, m_HorAcceleration{ 250.f }
	, m_JumpSpeed{ 180.f }
	, m_MaxJumpTime{ 0.15f }
	, m_JumpTime{ 0.f }
	, m_MacroState{ MacroState::basic }
	, m_Health{ 3 }
	, m_Lives{ 25 }
	, m_MaxHealth{ 6 }
	, m_IsInvulnerable{false}
	, m_CanSpitStar{false}
	, m_MaxParticleFrames{50}
	, m_GotDamaged{false}
	, m_HasReleasedJump{true}
	, m_HasReleasedR{true}
{
	Initialize();
}

Kirby::~Kirby()
{
	DeleteSprites();
	delete m_pStateHandler;
	m_pStateHandler = nullptr;

	for (SoundEffect* pSound : m_pSounds)
	{
		if (pSound) delete pSound;
	}
	m_pSounds.clear();
}

void Kirby::Initialize()
{
	InitializeSprites();
	
	// InitializeHurtSprites();
	m_pCurrentSprite = GetSpritePtr("kirby_idle");
	SetDimsFromSprite();

	m_pStateHandler = new KirbyStateHandler(this);
	m_pSounds.push_back(new SoundEffect{ "resources/sounds/spitstar.wav" });
}

void Kirby::InitializeSprites()
{
	int nrFrames{ 1 };
	float animationSpeed{ 0.f };
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_jumping" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_falling" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_inhaling" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_ability_jumping" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_ability_falling" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_ducking" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_sliding" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_ability_ducking" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_ability_sliding" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_ability_stone_continuous" });
	animationSpeed = 0.15f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_bloated_idle" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_ability_spark_start" });
	animationSpeed = 0.4f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_hurt" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_inflated_hurt" });

	nrFrames = 2;
	animationSpeed = 0.05f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_ability_fire_continuous" });
	animationSpeed = 0.1f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_ability_spark_continuous" });
	animationSpeed = 0.15f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_start_inhaling" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_exhaling" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_flapping" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_ability_flapping" });
	animationSpeed = 0.75f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_inflated" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_ability_inflated" });

	nrFrames = 3;
	animationSpeed = 0.2f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_ability_stone_start" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_ability_stone_end" });
	animationSpeed = 0.3f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_swallowing" });
	

	nrFrames = 4;
	animationSpeed = 0.2f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_flipping" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_ability_flipping" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_bloat" });
	animationSpeed = 0.4f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_inflating" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_deflating" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_ability_inflating" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_ability_deflating" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_spitting" });
	animationSpeed = 0.7f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_walking" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_ability_walking" });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_bloated_walking" });

	nrFrames = 16;
	animationSpeed = 1.6f;
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_idle", 2 });
	m_pSprites.push_back(new Sprite{ nrFrames, animationSpeed, "kirby_ability_idle", 2 });

	CreateAltSprites();
}

void Kirby::DeleteSprites()
{
	for (Sprite* pSprite : m_pSprites)
	{
		if (pSprite)
		{
			delete pSprite;
		}
	}
	m_pSprites.clear();
}

void Kirby::Draw() const
{
	if (m_MacroState == MacroState::inhalation)
	{
		Color4f transparentWhite{ 1.f, 1.f, 1.f, 0.3f };
		utils::SetColor(transparentWhite);
		utils::FillRect(GetInhalationZone());
	}
	GameObject::Draw();
}

void Kirby::Update(float elapsedSec)
{
	UpdateDamaged(elapsedSec);
	UpdateSprite(elapsedSec);
	SetIsOnGround();
	ProcessInput(elapsedSec);
	UpdateState();
	ProcessMovement(elapsedSec);
	LockToLevel();
	if (HasPowerUp())
	{
		if (m_MacroState == MacroState::bloated)
		{
			std::string spriteName = m_pCurrentSprite->GetName();
			Flicker(m_AccumulatedTime, spriteName);
		}
		GetPowerUp()->Update(elapsedSec);
	}
	
	float direction{};
	if (m_pProjectileManager->ProjectileHasHit(this, Projectile::ActorType::kirby, direction))
	{
		DecrementHealth(direction);
	}
}

#pragma region UpdateHelpers
void Kirby::SetIsOnGround()
{
	bool isAlreadyOnGround = m_IsOnGround;

	if (m_IsForcedOnGround == false)
	{
		Actor::SetIsOnGround();
	}
	else
	{
		m_IsForcedOnGround = false;
	}

	if (m_IsOnGround && m_HasReleasedJump)
	{
		m_JumpTime = 0.f;
	}

	CheckForShakeCommand(isAlreadyOnGround);
}

void Kirby::SetVerticalVelocityToZero()
{
	m_Velocity.y = 0.f;
}

void Kirby::ForceIsOnGround()
{
	m_IsForcedOnGround = true;
	m_IsOnGround = true;
}

void Kirby::UpdateState()
{
	m_pStateHandler->HandleState(m_ActionState, m_MacroState);
}

void Kirby::ProcessMovement(float elapsedSec)
{
	float xVelocity{ m_Velocity.x }, yVelocity{ m_Velocity.y };
	if (m_Velocity.y < 0.f)
	{
		m_JumpTime = m_MaxJumpTime;
	}
	if (m_MacroState == MacroState::inflated)
	{
		const float speedModifier{ 0.5f };
		xVelocity *= speedModifier;
		yVelocity *= speedModifier;
	}
	ApplyVelocities(elapsedSec, xVelocity, yVelocity);
	ApplyGravity(elapsedSec);
	HandleLevelCollisions();
}

void Kirby::LockToLevel()
{
	float& kirbyLeft{ m_Shape.left };
	float& kirbyBottom{ m_Shape.bottom };
	const float kirbyRight{ kirbyLeft + m_Shape.width };
	const float kirbyTop{ kirbyBottom + m_Shape.height };
	const float yOffsetScalar{ m_pCurrentSprite->GetFrameDimensions().y / m_Shape.height };
	const float topBorder{ yOffsetScalar * m_Shape.height };
	Rectf boundaries{ LevelManager::GetCurrentLevel()->GetBoundaries()};

	if (kirbyBottom > boundaries.height - topBorder)
	{
		kirbyBottom = boundaries.height - topBorder;
		m_Velocity.y = 0.f;
	}
	if (kirbyLeft < boundaries.left) kirbyLeft = boundaries.left;
	else if (kirbyRight > boundaries.width) kirbyLeft = boundaries.width - m_Shape.width;

	if (kirbyBottom + m_Shape.height < 0.f)
	{
		KillKirby();
	}
}
#pragma endregion

#pragma region InputHandling
void Kirby::ProcessInput(float elapsedSec)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	const bool isUsingPower{ HasPowerUp() ? GetPowerUp()->IsActive() : false};
	const bool isImmobile{
		isUsingPower
		|| m_MacroState == MacroState::inhalation
		|| (m_MacroState == MacroState::ducking && m_IsOnGround)
		|| m_MacroState == MacroState::sliding 
		|| m_ActionState == ActionState::hurt};
	
	const bool rightKeysDown{ pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D] };
	const bool leftKeysDown{  pStates[SDL_SCANCODE_LEFT]  || pStates[SDL_SCANCODE_A] };
	const bool upKeysDown{ (pStates[SDL_SCANCODE_UP] || pStates[SDL_SCANCODE_W]) };
	const bool downKeysDown{ pStates[SDL_SCANCODE_DOWN] || pStates[SDL_SCANCODE_S] };
	
	if (leftKeysDown)
	{
		DoLeftHeldActions(isImmobile, elapsedSec);		
	}
	else if (rightKeysDown)
	{
		DoRightHeldActions(isImmobile, elapsedSec);
	}
	else
	{
		SlowDown(elapsedSec);
	}

	if (pStates[SDL_SCANCODE_SPACE])
	{
		DoSpaceHeldActions(isImmobile, elapsedSec);
	}

	if (upKeysDown)
	{
		DoUpHeldActions(isImmobile, elapsedSec);
	}

	if (pStates[SDL_SCANCODE_R])
	{
		DoRHeldActions();
	}

	DoDownHeldActions(downKeysDown);
	// Looks different because not holding down also causes an action in case kirby is ducking (go back to basic stance)
	// Shouldn't this be a release action? ==> Investigate!!
}

void Kirby::ProcessKeyUp(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
	case SDLK_SPACE:
		DoSpaceUpActions();
		break;
	case SDLK_r:
		DoRUpActions();
		break;
	case SDLK_a:
		ExpelPower();
		break;
	}
}

void Kirby::ProcessKeyDown(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
	case SDLK_a:
		if (m_MacroState == MacroState::bloated) 
		{
			m_MacroState = MacroState::basic;
			m_CanSpitStar = false;
		}
		break;
	case SDLK_r:
		DoRDownActions();
		break;
	// TEST CODE
	case SDLK_DOWN:
	case SDLK_s:
		if (m_MacroState == MacroState::bloated)
		{
			m_MacroState = MacroState::basic;
			m_CanSpitStar = false;
		}
		break;
	case SDLK_SPACE:
		DoSpaceDownActions();
		break;
	}
}

#pragma region KeyEventHelpers
void Kirby::DoLeftHeldActions(bool isImmobile, float elapsedSec)
{
	const float left{ -1.f };
	const bool canAccelerate{ m_Velocity.x > -m_MaxHorSpeed };
	if (!isImmobile)
	{
		m_XDirection = left;
		Move(elapsedSec, canAccelerate);
	}
	else SlowDown(elapsedSec);
}

void Kirby::DoRightHeldActions(bool isImmobile, float elapsedSec)
{
	const float right{ 1.f };
	const bool canAccelerate{ m_Velocity.x < m_MaxHorSpeed };
	if (!isImmobile)
	{
		m_XDirection = right;
		Move(elapsedSec, canAccelerate);
	}
	else SlowDown(elapsedSec);
}

void Kirby::DoDownHeldActions(bool isKeyDown)
{
	const bool isUsingPower{ HasPowerUp() ? GetPowerUp()->IsActive() : false};
	if (isKeyDown && m_MacroState == MacroState::basic)
	{
		if (!isUsingPower)
		{
			m_MacroState = MacroState::ducking;
		}
	}
	else if (!isKeyDown && m_MacroState == MacroState::ducking)
	{
		m_MacroState = MacroState::basic;
	}
}

void Kirby::DoUpHeldActions(bool isImmobile, float elapsedSec)
{
	if (isImmobile) return;

	if (m_MacroState != MacroState::bloated)
	{
		Flap();
	}
	else if (CanJump())
	{
		Jump(elapsedSec);
	}
}

void Kirby::DoRDownActions()
{
	if (m_HasReleasedR == false) return; // Do not execute down events anymore if R hasn't gone up yet


	// Deflate if inflated; possible both with & without powerup, so handled first
	Door info{ LevelManager::GetCurrentLevel()->GetDoorInfo(m_Shape)};
	if (info.GetExitLevelName() != "")
	{
		m_pLevelManager->LoadLevel(info.GetExitLevelName());
		SetLocation(info.GetExitLocation());
	}
	else if (m_MacroState == MacroState::inflated)
	{
		m_Velocity.y = 0.f;
		m_MacroState = MacroState::basic;
		SpawnPuff();
	}
	// Use PowerUp; if no need to deflate, down event of powerup can be used
	else if (HasPowerUp() && m_MacroState == MacroState::basic)
	{
		GetPowerUp()->OnKeyDownEvent(m_Shape, m_XDirection);
		if (GetPowerUp()->GetType() == PowerUp::PowerUpType::stone)
		{
			ToggleRockMode();
		}
	}
	// Spit star; 
	else if (m_MacroState == MacroState::bloated)
	{
		if (m_CanSpitStar)
		{
			SpitStar();
		}
	}
	// Inhale; if all above conditions are not met, and Kirby has no powerup
	else if (HasPowerUp() == false)
	{
		m_MacroState = MacroState::inhalation;
	}
	m_HasReleasedR = false;
}
void Kirby::DoRHeldActions()
{
	if (HasPowerUp() && m_MacroState == MacroState::basic) // Use Powerup
	{
		GetPowerUp()->ContinuousKeyEvent(m_Shape, m_XDirection);
	}
}
void Kirby::DoRUpActions()
{
	if (m_MacroState == MacroState::bloated)
	{
		m_CanSpitStar = true;
	}
	else if (m_MacroState == MacroState::inhalation)
	{
		m_MacroState = MacroState::basic;
	}
	else if (HasPowerUp() && m_MacroState == MacroState::basic)
	{
		GetPowerUp()->OnKeyUpEvent(m_Shape, m_XDirection);
	}
	m_HasReleasedR = true;
}

void Kirby::DoSpaceDownActions()
{
	const bool isUsingPower{ HasPowerUp() ? GetPowerUp()->IsActive() : false };

	if (m_MacroState == MacroState::ducking && m_IsOnGround && !isUsingPower)
	{
		m_Velocity.x = 180.f * m_XDirection;
		m_MacroState = MacroState::sliding;
	}
}

void Kirby::DoSpaceHeldActions(bool isImmobile, float elapsedSec)
{
	if (isImmobile) return; // Being immobile should prevent code below from execution
	if (m_MacroState == MacroState::inflated)
	{
		Flap();
	}
	else if (CanJump() && m_MacroState != MacroState::ducking && m_MacroState != MacroState::sliding)
	{
		Jump(elapsedSec);
		m_HasReleasedJump = false;
	}
}

void Kirby::DoSpaceUpActions()
{
	m_JumpTime = m_MaxJumpTime;
	m_HasReleasedJump = true;
}
#pragma endregion
#pragma endregion

#pragma region MovementHelpers

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
	m_Velocity.y = m_JumpSpeed;
	m_JumpTime += elapsedSec;
}


bool Kirby::CanJump() const
{
	return m_JumpTime < m_MaxJumpTime;
}

void Kirby::Flap()
{
	m_MacroState = MacroState::inflated;
	m_Velocity.y = m_JumpSpeed;
	m_JumpTime = m_MaxJumpTime;
}
#pragma endregion

void Kirby::SpitStar()
{
	SetState(ActionState::spitting);
	m_MacroState = MacroState::basic;
	m_pSounds[0]->Play(0);

	// Copy shape first to manipulate copied shape properties underneath, then offset to match sprite center and not spawn inside Kirby
	Rectf spawnRect{ m_Shape };																
	const float yOffset{ (m_pCurrentSprite->GetFrameDimensions().y - m_Shape.height) / 2 }; 
	spawnRect.left += m_XDirection * m_Shape.width;	
	spawnRect.bottom += yOffset;
	m_pProjectileManager->Add(new Star{ spawnRect, m_XDirection });

	// Spitting out the star should remove the power
	DeletePowerUp();

	m_CanSpitStar = false; 
}

void Kirby::SpawnPuff()
{
	Rectf spawnLocation{ 0.f, m_Shape.bottom, m_Shape.width, m_Shape.height };
	spawnLocation.left = m_Shape.left + (m_XDirection > 0.f ? m_Shape.width : -m_Shape.width);
	m_pProjectileManager->Add(new Puff{ spawnLocation, m_XDirection });
}

void Kirby::ExpelPower()
{
	if (HasPowerUp())
	{
		PowerStar* pPowerStar{ new PowerStar{GetLocation()} };
		pPowerStar->SetDirection(-GetDirection());
		TransferPowerUp(pPowerStar);
		m_pObjectManager->AddItem(pPowerStar);
	}
}

void Kirby::ToggleRockMode()
{
	m_IsInvulnerable = !m_IsInvulnerable;

	if (m_IsOnGround)
	{
		const float bump{ 100.f };
		m_Velocity.y += bump;
	}
}

Rectf Kirby::GetInhalationZone() const
{
	// Return offscreen rectangle so no enemies get influenced
	if (m_MacroState != MacroState::inhalation)
	{
		return Rectf{ -100.f, -100.f, 0.f, 0.f };
	}

	Point2f inhalationZoneDims{ 56.f, 48.f };
	Rectf inhalationZone{ 0.f, m_Shape.bottom, inhalationZoneDims.x, inhalationZoneDims.y };
	inhalationZone.left = (m_XDirection > 0.f) ? m_Shape.left + m_Shape.width / 2 : m_Shape.left - inhalationZoneDims.x - m_Shape.width / 2;
	inhalationZone.bottom = (m_Shape.bottom + m_pCurrentSprite->GetFrameDimensions().y / 2) - (inhalationZoneDims.y / 2);
	return inhalationZone;
}

bool Kirby::IsBloated() const
{
	if (m_MacroState == MacroState::bloated)
	{
		return true;
	}
	return false;
}

bool Kirby::IsInvulnerable() const
{
	return m_IsInvulnerable;
}

void Kirby::SetBloated()
{
	SetState(ActionState::bloat);
	m_MacroState = MacroState::bloated;
}

#pragma region Utility
void Kirby::SetState(const ActionState& state)
{
	m_ActionState = state;
	std::string spriteName{ GetSpriteNameFromState(state) };
	m_pCurrentSprite = GetSpritePtr(spriteName);
	m_AccumulatedTime = 0;
	m_CurrentFrame = 0;
	m_HasLooped = false;
	m_LoopProgressTime = 0;
}

void Kirby::SetMacroState(const MacroState& macroState)
{
	m_MacroState = macroState;
}

void Kirby::SetObjectManager(ObjectManager* objMngr)
{
	m_pObjectManager = objMngr;
}

void Kirby::SetLevelManager(LevelManager* lvlMngr)
{
	m_pLevelManager = lvlMngr;
}

bool Kirby::CheckCollisionWith(Actor* pActor, utils::HitInfo& hitInfoReference, bool& isVerticalCollision)
{
	Rectf actorShape{ pActor->GetShape() };
	std::vector<Point2f> vertices;
	vertices.push_back(Point2f{ actorShape.left, actorShape.bottom });
	vertices.push_back(Point2f{ actorShape.left, actorShape.bottom + actorShape.height});
	vertices.push_back(Point2f{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height });
	vertices.push_back(Point2f{ actorShape.left + actorShape.width, actorShape.bottom });

	float height{ m_Shape.height };
	if (m_MacroState == MacroState::ducking || m_MacroState == MacroState::sliding)
	{
		// Kirby should have a smaller collision box when ducking or sliding
		height /= 2;
	}

	Point2f horizontalRayStart{m_Shape.left, m_Shape.bottom + height / 2};
	Point2f horizontalRayEnd{ m_Shape.left + m_Shape.width, m_Shape.bottom + height / 2};
	
	Point2f verticalRayStart{m_Shape.left + m_Shape.width / 2, m_Shape.bottom};
	Point2f verticalRayEnd{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + height };
	
	if (utils::Raycast(vertices, horizontalRayStart, horizontalRayEnd, hitInfoReference))
	{
		isVerticalCollision = false;
		return true;
	}
	if (utils::Raycast(vertices, verticalRayStart, verticalRayEnd, hitInfoReference))
	{
		isVerticalCollision = true;
		return true;
	}
	return false;
}

bool Kirby::CheckCollisionWith(Actor* pActor)
{
	utils::HitInfo throwAwayInfo{};
	bool throwAwayBool{};
	return CheckCollisionWith(pActor, throwAwayInfo, throwAwayBool);
}

std::string Kirby::GetSpriteNameFromState(const ActionState& state) const
{
	std::string spriteName{"kirby_"};

	if (HasPowerUp())
	{
		spriteName.append("ability_");
		if (GetPowerUp()->HasCompleteSpriteSet())
		{
			spriteName.append(GetPowerUp()->GetPowerSuffix());
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
		if (HasPowerUp() && !(GetPowerUp()->HasCompleteSpriteSet()))
		{
			spriteName.append(GetPowerUp()->GetPowerSuffix());
		}
		spriteName.append("start");
		break;
	case ActionState::power_continuous:
		if (HasPowerUp() && !(GetPowerUp()->HasCompleteSpriteSet()))
		{
			spriteName.append(GetPowerUp()->GetPowerSuffix());
		}
		spriteName.append("continuous");
		break;
	case ActionState::power_end:
		if (HasPowerUp() && !(GetPowerUp()->HasCompleteSpriteSet()))
		{
			spriteName.append(GetPowerUp()->GetPowerSuffix());
		}
		spriteName.append("end");
		break;
	case ActionState::hurt:
		if (m_MacroState == MacroState::bloated || m_MacroState == MacroState::inflated)
		{
			spriteName = "kirby_inflated_hurt";
		}
		else
		{
			spriteName = "kirby_hurt";
		}
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

//float Kirby::GetJumptime() const
//{
//	return m_JumpTime;
//}
//
//float Kirby::GetMaxJumpTime() const
//{
//	return m_MaxJumpTime;
//}

bool Kirby::HasLooped() const
{
	return m_HasLooped;
}

bool Kirby::IsOnGround() const
{
	return m_IsOnGround;
}

void Kirby::IncrementLives()
{
	++m_Lives;
}

void Kirby::DecrementHealth(float direction)
{
	if (m_IsInvulnerable) return;

	SetState(ActionState::hurt);
	BounceOffInDirection(direction);

	--m_Health;
	m_IsInvulnerable = true;
	m_GotDamaged = true;
	ExpelPower();
	if (m_Health <= 0)
	{
		KillKirby();
	}
}

void Kirby::FullyHeal()
{
	m_Health = m_MaxHealth;
}

void Kirby::UpdateDamaged(float elapsedSec)
{
	if (!m_GotDamaged) return;
	
	std::string spriteName = m_pCurrentSprite->GetName();

	m_ArbitraryTimer += elapsedSec;
	Flicker(m_ArbitraryTimer, spriteName);

	const float flickerDuration{ 3.f };

	if (m_ArbitraryTimer > flickerDuration)
	{
		SetVulnerable(spriteName);
		ResetArbitraryTimer();
		m_GotDamaged = false;
	}
}

void Kirby::SetVulnerable(std::string spriteName)
{
	int splitIndex{ int(spriteName.find_last_of('_')) };
	m_IsInvulnerable = false;
	if (spriteName.substr(splitIndex) == "_alt")
	{
		spriteName = spriteName.substr(0, splitIndex);
		m_pCurrentSprite = GetSpritePtr(spriteName);
	}
}

void Kirby::KillKirby()
{
	--m_Lives;
	m_Health = m_MaxHealth;
	SetLocation(LevelManager::GetCurrentLevel()->GetStartLocation());
	m_Velocity.y = 0;
	m_Velocity.x = 0;
	DeletePowerUp();
	m_CanSpitStar = false;
	m_IsInvulnerable = false;
	m_ActionState = ActionState::idle;
	m_MacroState = MacroState::basic;
}

void Kirby::CheckForShakeCommand(bool isAlreadyOnGround)
{
	bool isOnGround = m_IsOnGround;

	if (HasPowerUp()) {
		if (GetPowerUp()->IsActive() && GetPowerUp()->GetType() == PowerUp::PowerUpType::stone)
		{
			if (isAlreadyOnGround != isOnGround && m_IsOnGround)
			{
				Camera::SetShake();
			}
		}
	}
}

void Kirby::BounceOffInDirection(float bounceDirection)
{
	Point2f velocity{ 150.f, 50.f };
	m_Velocity.x = velocity.x * bounceDirection;
	m_Velocity.y = velocity.y;
}

//bool Kirby::IsInhaling() const
//{
//	if (m_MacroState == MacroState::inhalation)
//	{
//		return true;
//	}
//	return false;
//}

//void Kirby::EnforceState()
//{
//	SetState(m_ActionState);
//}

void Kirby::SetPowerState()
{
	if (GetPowerUp()->HasStart())
	{
		SetState(ActionState::power_start);
	}
	else if (GetPowerUp()->HasContinuous())
	{
		SetState(ActionState::power_continuous);
	}
	else if (GetPowerUp()->HasEnd())
	{
		SetState(ActionState::power_end);
	}
}

