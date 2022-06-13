#pragma once
#include "GameObject.h"
#include "Vector2f.h"
#include "Actor.h"
class Sprite;
class LevelManager;

class Projectile : public GameObject
{
public:
	enum class ProjectileType {
		fireball,
		star,
		puff,
		spark,
		throwingStar,
		soundwave
	};

	explicit Projectile(Actor* pOwner, LevelManager* pLevelMngr, ProjectileType type, const Rectf& projectileRect, const Vector2f& velocity);
	// No dynamically allocated memory so rule of five implementation is not needed, default copy/move constructors and destructor are fine 

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
	LevelManager* m_pLevelManager;
	
	void ApplyVelocities(float elapsedSec);
	void DestroyOnCollision();
};

