#pragma once
#include "Projectile.h"
#include "Sprite.h"
// CLEANED INCLUDES

class ObjectManager;
class Actor;

class ProjectileManager final
{
public:
	ProjectileManager();
	ProjectileManager(const ProjectileManager& other) = delete;
	ProjectileManager& operator=(const ProjectileManager& other) = delete;
	ProjectileManager(ProjectileManager&& other) = delete;
	ProjectileManager& operator=(ProjectileManager&& other) = delete;
	~ProjectileManager();

	// static ProjectileManager* GetProjectileMngr();

	void Draw() const;
	void Update(float elapsedSec);

	void Add(Projectile* projectile);
	// This first version of ProjectileHasHit gets used to set the direction in which kirby should bounce off, reference to change direction within kirby
	bool ProjectileHasHit(Actor* pActor, float& direction);
	bool ProjectileHasHit(Actor* pActor);

private:
	// static ProjectileManager* m_pProjectileManager;

	std::vector<Projectile*> m_pProjectiles;
	std::vector<Sprite*> m_pSprites;

	void InitializeSprites();
	bool HasCollided(const Actor* pActor, const Projectile* pProjectile);

	void UpdateProjectile(float elapsedSec, int index);
	void DeleteProjectile(int index);
};

