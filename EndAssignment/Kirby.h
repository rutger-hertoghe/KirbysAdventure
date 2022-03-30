#pragma once
#include "Vector2f.h"
#include "Level.h"
#include "Actor.h"


class Sprite;
class Texture;
class PowerUp;
class Level;
class ProjectileManager;
class GameObject;

class Kirby final : public Actor
{
	enum class MacroState {
		basic,
		inflated,
		inhalation,
		bloated,
		ducking,
		sliding
	};
	enum class ActionState {
		idle,
		walking,
		jumping,
		flipping,
		falling,
		ducking,
		sliding,
		inflating,
		inflated,
		flapping,
		deflating,
		start_inhaling,
		inhaling,
		swallowing,
		exhaling,
		bloated_idle,
		bloated_walking,
		bloat,
		spitting,
		power_start,
		power_continuous,
		power_end,
		inflated_hurt,
		hurt
	};

public:
	Kirby();
	Kirby(const Kirby& other) = delete;
	Kirby& operator=(const Kirby& other) = delete;
	~Kirby() override;

	virtual void Update(float elapsedSec) override;
	void ProcessKeyUp(const SDL_KeyboardEvent& e);
	void ProcessKeyDown(const SDL_KeyboardEvent& e);

	void EnforceState();

	int GetHealth() const;
	int GetLives() const;

	void SetProjectileManager(ProjectileManager* pProjectileMgr);
	void Hit();

	void DecrementHealth();
	void KillKirby();

	virtual void Draw() const override;
	bool IsInhaling() const;
	Rectf GetInhalationZone() const;
	void SetBloated();

private:
	// Primitives
	const float m_MaxHorSpeed;
	const float m_JumpSpeed;
	const float m_HorAcceleration;
	const float m_MaxJumpTime;
	const int m_MaxHealth;
	const int m_MaxParticleFrames;

	bool m_IsInvulnerable;
	bool m_CanSpitStar;

	int m_Health;
	int m_Lives;
	int m_ParticleFrame;

	float m_ElapsedSec;
	float m_JumpTime;

	// Pointers
	ProjectileManager* m_pProjectileManager;

	// Non-Primitves
	Rectf m_SuctionZone;
	ActionState m_ActionState;
	MacroState m_MacroState;	

	// Functions
	void Initialize();
	void InitializeSprites();
	void DeleteSprites();

	void UpdateState();
	void ProcessInput(float elapsedSec);
	void ProcessMovement(float elapsedSec);

	void Flap();
	void Move(float elapsedSec, bool canAccelerate);
	void SlowDown(float elapsedSec);
	void Jump(float elapsedSec);
	void AccelerateJump(float elapsedSec);

	void DiscardPower();
	void SetPowerState();

	void SetState(const ActionState& state);
	void LockToLevel();
	std::string GetSpriteNameFromState(const ActionState& state) const;
	Sprite* GetSpritePtr(const std::string& spriteName) const;
	virtual void SetIsOnGround() override;
	
	void SpitStar();
	void SpawnPuff();

	void DoRDownActions();
	void DoRHeldActions();
	void DoRUpActions();

	void DoSpaceDownActions();
	void DoSpaceHeldActions(bool isImmobile, float elapsedSec);
	void DoSpaceUpActions();

	void DoLeftHeldActions(bool isImmobile, float elapsedSec);
	void DoRightHeldActions(bool isImmobile, float elapsedSec);
	void DoDownHeldActions(bool isKeyDown);
	void DoUpHeldActions(bool isImmobile, float elapsedSec);
};

