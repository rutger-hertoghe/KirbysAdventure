#pragma once
#include "Projectile.h"
#include "Actor.h"

class ProjectileManager;

class PowerUp
{
public:
	enum class PowerUpType
	{
		fire,
		beam,
		laser,
		mike,
		parasol,
		spark,
		stone,
		sword
	};

	// TODO: Implement parasol & mike powerup (mike is lowest priority)

	explicit PowerUp(PowerUpType type, Actor* pActor, bool hasStart, bool hasContinuous, bool hasEnd, bool hasCompleteSpriteSet = false);

	virtual void OnKeyDownEvent()		= 0;
	virtual void ContinuousKeyEvent()	= 0;
	virtual void OnKeyUpEvent()			= 0;

	virtual void Update(float elapsedSec) = 0;

	virtual bool IsActive() const = 0;
	// virtual bool IsOneShot();
	virtual int GetPowerPanelSlot() const;
	virtual std::string GetPowerSuffix() const = 0;
	PowerUpType GetType() const;

	bool HasStart() const;
	bool HasContinuous() const;
	bool HasEnd() const;
	bool HasCompleteSpriteSet() const;

	void SetOwner(Actor* pOwner);
	void SetInactive();

protected:
	const bool m_HasStart;
	const bool m_HasContinuous;
	const bool m_HasEnd;
	const bool m_HasCompleteSpriteSet;
	Actor* m_pOwner;

	bool m_IsActive;
	PowerUpType m_Type;
};

