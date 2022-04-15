#pragma once

class Enemy;
class Kirby;
class ProjectileManager;
class Level;
class Camera;
class Item;

class ObjectManager final
{
public:
	explicit ObjectManager(Kirby* pKirby);
	ObjectManager(const ObjectManager& other) = delete;
	ObjectManager& operator=(const ObjectManager& other) = delete;
	ObjectManager(ObjectManager&& other) = delete;
	ObjectManager& operator=(ObjectManager&& other) = delete;
	~ObjectManager();

	void Draw() const;
	void Update(float elapsedSec, const Rectf& visibleArea);

	void SetLevelPointers(Level* levelPtr);

	void AddItem(Item* pItem);

	void ClearEnemyVector();
	void LoadEnemiesByLevelName(const std::string& levelName);

	// TESTING PURPOSES
	void ResetEnemies();


private:
	Rectf m_VisibleArea;

	std::vector<Enemy*> m_pEnemies;
	std::vector<Item*> m_pItems;

	Kirby* m_pKirby;
	// OBJECTMANAGER OWNS THE PROJECTILE MANAGER AND MANAGES IT
	ProjectileManager* m_pProjectileManager;

	void CheckEnemyRemovalConditions(Enemy*& pEnemy, bool insideXScreenBounds, bool insideYScreenBounds);
	void UpdateEnemies(float elapsedSec);
	void UpdateItems(float elapsedSec);
	
	void UpdateEnemy(Enemy*& pEnemy, float elapsedSec);
	void UpdateItem(Item*& pItem, float elapsedSec);

	void DeleteEnemies();
	void DeleteItems();

	void LoadEnemiesFromFile(const std::string& filePath);
	void CreateEnemy(const std::string& enemyName, const Point2f& location);

	void SetEnemyProjectileManagerPointers();

	bool InsideXScreenBounds(const Rectf& shape);
	bool InsideYScreenBounds(const Rectf& shape);
};

