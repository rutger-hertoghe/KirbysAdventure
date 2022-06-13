#pragma once
#include "Vector2f.h"
#include "Level.h"
#include "Actor.h"
#include "utils.h"

class Sprite;
class Texture;
class PowerUp;
class Level;
class ProjectileManager;
class GameObject;
class KirbyStateHandler;
class SoundEffect;
class LevelManager;
class ObjectManager;
class SoundStream;

class Kirby final : public Actor
{
public:
	enum class MacroState {
		basic,
		inflated,
		inhalation,
		bloated,
		ducking,
		sliding,
		dead
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
		hurt,
		dead
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

	void SetLevelManager(LevelManager* pLevelManager);
	void SetProjectileManager(ProjectileManager* pProjectileManager);

	int GetHealth() const;
	int GetMaxHealth() const;
	int GetLives() const;
	int GetScore() const;
	void AddScore(int score);

	bool HasLooped() const;

	void IncrementLives();
	void DecrementHealth(float direction);
	void FullyHeal();

	virtual void Draw() const override;
	Rectf GetInhalationZone() const;

	bool IsBloated() const;
	bool IsInvulnerable() const;
	bool IsDead() const;

	void SetBloated();

	void SetPowerState();
	void SetState(const ActionState& state);
	void SetMacroState(const MacroState& macroState);

	bool CheckCollisionWith(Actor* pActor);
	bool CheckCollisionWith(Actor* pActor, utils::HitInfo& hitInfoReference, bool& isVerticalCollision);

	void ForceIsOnGround();
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
	bool m_IsForcedOnGround;

	bool m_HasReleasedJump;
	bool m_HasReleasedR;

	bool m_WentThroughDoor;

	bool m_CanSpitStar;

	int m_Health;
	int m_Lives;
	int m_Score;

	float m_ElapsedSec;
	float m_JumpTime;

	// Pointers
	KirbyStateHandler* m_pStateHandler;
	SoundStream* m_pDeathSound;

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
	virtual void SetIsOnGround() override;

	void Flap();
	void Move(float elapsedSec, bool canAccelerate);
	void SlowDown(float elapsedSec);
	void Jump(float elapsedSec);
	void BounceOffInDirection(float bounceDirection);
	bool CanJump() const;

	void UpdateDamaged(float elapsedSec);
	void SetVulnerable(std::string spriteName);
	void CheckHitByProjectile();
	void LockToLevel();
	std::string GetSpriteNameFromState(const ActionState& state) const;
	
	void SpitStar();
	void SpawnPuff();
	void ExpelPower();
	void ToggleRockMode();
	void KillKirby();
	void ResetKirby();
	void CheckForShakeCommand(bool isAlreadyOnGround);

	void DoEDownActions();
	void DoEHeldActions();
	void DoEUpActions();

	void DoSpaceDownActions();
	void DoSpaceHeldActions(bool isImmobile, float elapsedSec);
	void DoSpaceUpActions();

	void DoLeftHeldActions(bool isImmobile, float elapsedSec);
	void DoRightHeldActions(bool isImmobile, float elapsedSec);
	void DoDownHeldActions(bool isKeyDown);
	void DoUpHeldActions(bool isImmobile, float elapsedSec);
};

