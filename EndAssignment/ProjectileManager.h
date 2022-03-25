#pragma once
#include "Projectile.h"
#include "Sprite.h"

class Actor;

class ProjectileManager final
{
public:
	ProjectileManager();
	~ProjectileManager();

	void Draw() const;
	void Update(float elapsedSec);

	void Add(Projectile* projectile);
	bool ProjectileHasHit(Actor* pActor);
private:
	std::vector<Projectile*> m_pProjectiles;
	std::vector<Sprite*> m_pSprites;

	void InitializeSprites();
	bool HasCollided(const Actor* pActor, const Projectile* pProjectile);
};

