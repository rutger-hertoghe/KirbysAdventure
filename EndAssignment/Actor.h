#pragma once
#include "GameObject.h"
#include "Level.h"

class ProjectileManager;

class Actor : public GameObject
{
public:
	Actor();
	virtual ~Actor() override;
	virtual void Update(float elapsedSec) = 0;

	// POWERUP JUGGLING
	void TransferPowerUp(Actor* newOwner);
	virtual void SetPowerUp(PowerUp* powerUp, ProjectileManager* pProjectileMgr);
	bool HasPower();

	Point2f GetLocation() const;
	PowerUp* GetPowerUp() const;

	void SetCurrentLevel(Level* level);
	virtual void SetIsOnGround();

	void SetInhalationVelocities(const Rectf& kirbyRect);
	bool IsInhalable();
	bool IsBeingInhaled();
	void ToggleBeingInhaled(const Rectf& inhalationZone);


protected:
	static const float m_Gravity;
	
	bool m_IsOnGround;

	Vector2f m_BaseVelocity;
	Vector2f m_Velocity;

	bool m_IsInhalable;
	bool m_IsBeingInhaled;
	
	PowerUp* m_pPowerUp;
	Level* m_pCurrentLevel;

	void SetBaseVelocity(float xVelocity, float yVelocity);
	void SetBaseVelocity(const Vector2f& velocity);
	void SetBaseVelocity(const Point2f& velocity);
	void ApplyVelocities(float elapsedSec, float xVelocity, float yVelocity);
	void ApplyGravity(float elapsedSec);
	void HandleCollisions();

};

