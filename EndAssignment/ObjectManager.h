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
	explicit ObjectManager();
	ObjectManager(const ObjectManager& other) = delete;
	ObjectManager& operator=(const ObjectManager& other) = delete;
	ObjectManager(ObjectManager&& other) = delete;
	ObjectManager& operator=(ObjectManager&& other) = delete;
	~ObjectManager();

	void Draw() const;
	void Update(float elapsedSec, const Point2f& cameraLocation);
	void LinkKirby(Kirby* kirbyPtr);

	void SetLevelPointers(Level* levelPtr);
	void SupplyViewportDimensions(const Point2f& viewDimensions);
	std::vector<Enemy*>& GetEnemyVector();

	void AddItem(Item* pItem);

	void ClearEnemyVector();
	void LoadEnemiesByLevelName(const std::string& levelName);

private:
	Point2f m_ViewSize;
	Point2f m_ViewLocation;

	std::vector<Enemy*> m_pEnemies;
	std::vector<Item*> m_pItems;

	Kirby* m_pKirby;
	ProjectileManager* m_pProjectileManager;

	void CheckEnemyRemovalConditions(Enemy*& pEnemy);
	void UpdateEnemy(Enemy*& pEnemy, float elapsedSec);
	void UpdateItem(Item*& pItem, float elapsedSec);

	void DeleteEnemies();
	void DeleteItems();

	void LoadEnemiesFromFile(const std::string& filePath);
	void CreateEnemy(const std::string& enemyName, const Point2f& location);

	void SetEnemyProjectileManagerPointers();
};

