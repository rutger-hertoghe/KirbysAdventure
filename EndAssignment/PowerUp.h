#pragma once
#include "Projectile.h"

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

	// TODO: Implement stone, parasol & mike powerup

	explicit PowerUp(PowerUpType type, ProjectileManager* pMgr, bool hasStart, bool hasContinuous, bool hasEnd, bool hasCompleteSpriteSet = false);

	virtual void OnKeyDownEvent(const Rectf& shape, float xDirection)		= 0;
	virtual void ContinuousKeyEvent(const Rectf& shape, float xDirection)	= 0;
	virtual void OnKeyUpEvent(const Rectf& shape, float xDirection)		= 0;

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

	void SetOwnerType(Projectile::ActorType owner);
	void SetInactive();

protected:
	const bool m_HasStart;
	const bool m_HasContinuous;
	const bool m_HasEnd;
	const bool m_HasCompleteSpriteSet;
	Projectile::ActorType m_Owner;

	bool m_IsActive;
	PowerUpType m_Type;
	ProjectileManager* m_pProjectileManager;
};

