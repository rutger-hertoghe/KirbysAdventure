#pragma once
#include "Projectile.h"
#include "Sprite.h"

class ObjectManager;
class Actor;

class ProjectileManager final
{
public:
	ProjectileManager();
	ProjectileManager(const ProjectileManager& other) = delete;
	ProjectileManager& operator=(const ProjectileManager& other) = delete;
	~ProjectileManager();

	void Draw() const;
	void Update(float elapsedSec);

	void Add(Projectile* projectile);
	bool ProjectileHasHit(Actor* pActor, Projectile::ActorType hitActorType, float& direction);

private:
	std::vector<Projectile*> m_pProjectiles;
	std::vector<Sprite*> m_pSprites;

	void InitializeSprites();
	bool HasCollided(const Actor* pActor, const Projectile* pProjectile);
};

