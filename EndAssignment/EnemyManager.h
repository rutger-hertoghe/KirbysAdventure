#pragma once

class Enemy;
class Kirby;
class ProjectileManager;
class Level;

class EnemyManager final
{
public:
	EnemyManager();
	EnemyManager(const EnemyManager& other) = delete;
	EnemyManager& operator=(const EnemyManager& other) = delete;
	~EnemyManager();

	void Draw() const;
	void Update(float elapsedSec);
	void LinkProjectileManager(ProjectileManager* managerPtr);
	void LinkKirby(Kirby* kirbyPtr);
	void LinkLevel(Level* levelPtr);
	void SupplyViewportDimensions(const Point2f& viewDimensions);

private:
	Point2f viewSize{};

	std::vector<Enemy*> m_pEnemies;

	Kirby* m_pKirby;
	ProjectileManager* m_pProjectileManager;

	void Initialize();
	void CheckEnemyRemovalConditions(Enemy*& pEnemy);
	void UpdateEnemy(Enemy*& pEnemy, float elapsedSec);
	
};

