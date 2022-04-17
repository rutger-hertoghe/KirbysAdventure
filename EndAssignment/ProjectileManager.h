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

	static ProjectileManager* GetProjectileMngr();

	void Draw() const;
	void Update(float elapsedSec);

	void Add(Projectile* projectile);
	// This first version of ProjectileHasHit gets used to set the direction in which kirby should bounce off
	bool ProjectileHasHit(Actor* pActor, Projectile::ActorType hitActorType, float& direction);
	bool ProjectileHasHit(Actor* pActor, Projectile::ActorType hitActorType);

private:
	static ProjectileManager* m_pProjectileManager;

	std::vector<Projectile*> m_pProjectiles;
	std::vector<Sprite*> m_pSprites;

	void InitializeSprites();
	bool HasCollided(const Actor* pActor, const Projectile* pProjectile);
};

