#pragma once
#include "GameObject.h"
#include "Vector2f.h"
class Sprite;

class Projectile : public GameObject
{
public:
	enum class ProjectileType {
		fireball,
		star,
		puff,
		spark,
		throwingStar
	};

	enum class ActorType {
		kirby,
		enemy
	};

	explicit Projectile(ActorType owner, ProjectileType type, const Rectf& projectileRect, const Vector2f& velocity);

	virtual void Update(float elapsedSec) = 0;
	void SetSprite(Sprite* spritePtr);

	bool IsReadyToDestroy() const;
	bool IsPersistent() const;
	void SetReadyToDestroy();

	int GetTypeInt() const;
	ActorType GetOwner();

protected:
	bool m_IsReadyToDestroy;
	bool m_IsPersistent;

	Vector2f m_Velocity;
	ProjectileType m_Type;
	ActorType m_Owner;
	
	void ApplyVelocities(float elapsedSec);
};

