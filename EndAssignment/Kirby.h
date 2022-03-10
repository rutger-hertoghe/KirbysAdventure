#pragma once
#include <vector>
#include "Vector2f.h"
#include "Level.h"
#include "GameObject.h"

class Sprite;
class Texture;
class PowerUp;

class Kirby : public GameObject
{
	enum class ActionState {
		idle,
		walking,
		jumping,
		flipping,
		falling,
		ducking,
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
		power_end
	};

public:
	Kirby();
	~Kirby();

	void Draw() const;
	void Update(float elapsedSec, const Level& level);
	void ProcessKeyUp(const SDL_KeyboardEvent& e);
	void ProcessKeyDown(const SDL_KeyboardEvent& e);

	void EnforceState();

	int GetHealth() const;
	int GetLives() const;
	Point2f GetLocation() const;
	PowerUp* GetPowerUp() const;
private:
	const float m_MaxHorSpeed;
	const float m_JumpSpeed;
	const float m_HorAcceleration;
	const float m_Gravity;
	const float m_MaxJumpTime;
	const int m_MaxHealth;

	int m_Health;
	int m_Lives;

	float m_JumpTime;
	bool m_IsOnGround;
	
	bool m_IsInflated;
	bool m_IsInhaling;
	bool m_IsBloated;
	bool m_IsDucking;

	Vector2f m_Velocity;
	ActionState m_ActionState;

	PowerUp* m_pPowerup;
	Sprite* m_pCurrentSprite;
	std::vector<Sprite*> m_pSprites;

	void InitializeSprites();
	void DeleteSprites();

	void UpdateState();
	void ProcessInput(float elapsedSec, const Level& level);
	void ProcessMovement(float elapsedSec, const Level& level);

	void Flap();
	void MoveLeft(float elapsedSec);
	void MoveRight(float elapsedSec);
	void Move(float elapsedSec, bool canAccelerate);
	void SlowDown(float elapsedSec);
	void Jump(float elapsedSec);
	void AccelerateJump(float elapsedSec);

	void DiscardPower();
	void SetPowerState();

	void SetIsOnGround(const Level& level);
	void SetState(const ActionState& state);
	void LockToLevel(const Level& level);
	std::string GetSpriteNameFromState(const ActionState& state) const;
	Sprite* GetSpritePtr(const std::string& spriteName) const;

};

