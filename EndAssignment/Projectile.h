#pragma once
#include "GameObject.h"
#include "Vector2f.h"
#include "Actor.h"
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

	explicit Projectile(Actor* pOwner, ProjectileType type, const Rectf& projectileRect, const Vector2f& velocity);

	virtual void Update(float elapsedSec) = 0;
	void SetSprite(Sprite* spritePtr);

	bool IsReadyToDestroy() const;
	bool IsPersistent() const;
	void SetReadyToDestroy();

	int GetTypeInt() const;
	Actor* GetOwner();

protected:
	bool m_IsReadyToDestroy;
	bool m_IsPersistent;

	Vector2f m_Velocity;
	ProjectileType m_Type;
	Actor* m_pOwner;
	
	void ApplyVelocities(float elapsedSec);
	void DestroyOnCollision();
};

