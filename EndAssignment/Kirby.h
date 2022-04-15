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
class KirbyStateHandler;
class SoundEffect;
class LevelManager;

class Kirby final : public Actor
{
public:
	enum class MacroState {
		basic,
		inflated,
		inhalation,
		bloated,
		ducking,
		sliding
	};

	// TODO: Finalize kirby states & movement, fix bugs & clean code

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

	explicit Kirby();
	Kirby(const Kirby& other) = delete;
	Kirby& operator=(const Kirby& other) = delete;
	Kirby(Kirby&& other) = delete;
	Kirby& operator=(Kirby&& other) = delete;
	~Kirby() override;

	virtual void Update(float elapsedSec) override;
	void ProcessKeyUp(const SDL_KeyboardEvent& e);
	void ProcessKeyDown(const SDL_KeyboardEvent& e);

	//void EnforceState();

	int GetHealth() const;
	int GetLives() const;

	//float GetJumptime() const;
	//float GetMaxJumpTime() const;

	bool IsOnGround() const;
	bool HasLooped() const;

	void DecrementHealth(float direction);

	virtual void Draw() const override;
	// bool IsInhaling() const;
	Rectf GetInhalationZone() const;

	bool IsBloated() const;
	bool IsInvulnerable() const;
	void SetBloated();
	void SetPowerState();
	void SetState(const ActionState& state);
	void SetMacroState(const MacroState& macroState);
	void SetLevelManager(LevelManager* lvlMngr);
	
	bool GiveShakeCommand();

	bool CheckCollisionWith(Actor* pActor);

private:
	// Primitives
	const float m_MaxHorSpeed;
	const float m_JumpSpeed;
	const float m_HorAcceleration;
	const float m_MaxJumpTime;
	const int m_MaxHealth;
	const int m_MaxParticleFrames;

	bool m_IsInvulnerable;
	bool m_GotDamaged;
	bool m_ShakeCommand;

	bool m_HasReleasedJump;
	bool m_HasReleasedR;

	bool m_CanSpitStar;

	int m_Health;
	int m_Lives;
	int m_ParticleFrame;

	float m_ElapsedSec;
	float m_JumpTime;

	// Pointers
	KirbyStateHandler* m_pStateHandler;
	LevelManager* m_pLevelManager;
	std::vector<SoundEffect*> m_pSounds;

	// Non-Primitves
	Rectf m_SuctionZone;
	ActionState m_ActionState;
	MacroState m_MacroState;	


	// Functions
	void Initialize();
	void InitializeSprites();
	// void InitializeHurtSprites();
	void DeleteSprites();

	void UpdateState();
	void ProcessInput(float elapsedSec);
	void ProcessMovement(float elapsedSec);

	void Flap();
	void Move(float elapsedSec, bool canAccelerate);
	void SlowDown(float elapsedSec);
	void Jump(float elapsedSec);
	void BounceOffInDirection(float bounceDirection);
	bool CanJump() const;

	void UpdateDamaged(float elapsedSec);
	void SetVulnerable(std::string spriteName);
	
	void LockToLevel();
	std::string GetSpriteNameFromState(const ActionState& state) const;
	virtual void SetIsOnGround() override;
	
	void SpitStar();
	void SpawnPuff();
	void ToggleRockMode();
	void KillKirby();
	void CheckForShakeCommand(bool isAlreadyOnGround);

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

