#pragma once
#include "utils.h"
#include "RemovalFX.h"

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

	static ObjectManager* GetObjectMngr();

	void Draw() const;
	void Update(float elapsedSec, const Rectf& visibleArea);

	void AddItem(Item* pItem);
	void AddRemovalFX(const Point2f& location, RemovalFX::FXType type);

	void ClearEnemyVector();
	void ClearObjectVector();
	void LoadObjectsByLevelName(const std::string& levelName);

	// TESTING PURPOSES
	// void ResetEnemies();

private:
	static ObjectManager* m_pObjectManager;

	Rectf m_VisibleArea;

	std::vector<Enemy*> m_pEnemies;
	std::vector<Item*> m_pItems;
	std::vector<RemovalFX*> m_pRemovalFX;

	Kirby* m_pKirby;

	void DrawEnemies() const;
	void DrawItems() const;
	void DrawFXs() const;

	void UpdateEnemies(float elapsedSec);
	void UpdateItems(float elapsedSec);
	void UpdateRemovalFXs(float elapsedSec);
	
	void UpdateEnemy(Enemy*& pEnemy, float elapsedSec);
	void CheckEnemyRemovalConditions(Enemy*& pEnemy, bool insideXScreenBounds, bool insideYScreenBounds);
	void UpdateItem(Item*& pItem, float elapsedSec);
	void DoItemInhalationActions(Item*& pItem);
	void DoItemCollision(Item*& pItem, bool isVerticalCollision, const utils::HitInfo& hitInfo);

	void DeleteEnemies();
	void DeleteItems();
	void DeleteFXs();

	void LoadObjectsFromFile(const std::string& filePath);
	void CreateObject(const std::string& objectName, const Point2f& location);

	void SetEnemyProjectileManagerPointers();

	bool InsideXScreenBounds(const Rectf& shape);
	bool InsideYScreenBounds(const Rectf& shape);
};

