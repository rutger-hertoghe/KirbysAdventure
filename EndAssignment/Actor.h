#pragma once
#include "GameObject.h"
#include "Level.h"

class ProjectileManager;
class PowerUp;

class Actor : public GameObject
{
public:
	explicit Actor();
	Actor(const Actor& other) = delete;
	Actor& operator=(const Actor& other) = delete;
	Actor(Actor&& other) = delete;
	Actor& operator=(Actor&& other) = delete;
	virtual ~Actor() override;

	virtual void Update(float elapsedSec) = 0;

	// POWERUP JUGGLING
	void TransferPowerUp(Actor* newOwner);
	virtual void SetPowerUp(PowerUp* powerUp);
	bool HasPowerUp() const;
	void DeletePowerUp();

	void SetLocation(const Point2f& location);
	void SetLocation(float x, float y);
	void AddVelocity(float x, float y);
	Vector2f GetVelocity() const;
	Point2f GetLocation() const;
	PowerUp* GetPowerUp() const;

	void SetInhalationVelocities(const Rectf& kirbyRect);
	bool IsInhalable() const;
	bool IsBeingInhaled() const;
	bool IsOnGround() const;
	void ToggleBeingInhaled(const Rectf& inhalationZone);

protected:
	static const float m_Gravity;
	
	float m_ArbitraryTimer;

	bool m_IsOnGround;

	Vector2f m_BaseVelocity;
	Vector2f m_Velocity;

	bool m_IsInhalable;
	bool m_IsBeingInhaled;

	PowerUp* m_pPowerUp;

	void SetBaseVelocity(float xVelocity, float yVelocity);
	void SetBaseVelocity(const Vector2f& velocity);
	void SetBaseVelocity(const Point2f& velocity);
	virtual void SetIsOnGround();
	void ApplyVelocities(float elapsedSec, float xVelocity, float yVelocity);
	void ApplyGravity(float elapsedSec);
	void HandleLevelCollisions();
	void ResetArbitraryTimer();

	void Flicker(float timer, std::string& spriteName);
	void CreateAltSprites();

	void ChangeDirectionOnBump();
};

