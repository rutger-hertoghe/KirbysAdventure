#pragma once
#include "utils.h"
#include "RemovalFX.h"

class Enemy;
class Kirby;
class ProjectileManager;
class Level;
class Camera;
class Item;
class MrTickTock;
class LevelManager;

class ObjectManager final
{
public:
	explicit ObjectManager(Kirby* pKirby, LevelManager* pLevelManager, ProjectileManager* pProjectileManager);
	ObjectManager(const ObjectManager& other) = delete;
	ObjectManager& operator=(const ObjectManager& other) = delete;
	ObjectManager(ObjectManager&& other) = delete;
	ObjectManager& operator=(ObjectManager&& other) = delete;
	~ObjectManager();

	static ObjectManager* GetObjectMngr();

	void Draw() const;
	void Update(float elapsedSec, Camera* pCamera);

	void AddItem(Item* pItem);
	void AddRemovalFX(const Point2f& location, RemovalFX::FXType type);

	void LoadObjectsByLevelName(const std::string& levelName);

	void Clear();
	
	Rectf GetVisibleArea() const;

private:
	static ObjectManager* m_pObjectManager;

	Rectf m_VisibleArea;

	std::vector<Enemy*> m_pEnemies;
	std::vector<Item*> m_pItems;
	std::vector<RemovalFX*> m_pRemovalFX;

	MrTickTock* m_pMrTickTock;
	Kirby* m_pKirby;
	LevelManager* m_pLevelManager;
	ProjectileManager* m_pProjectileManager;

	void DrawEnemies() const;
	void DrawItems() const;
	void DrawFXs() const;

	void UpdateEnemies(float elapsedSec);
	void UpdateItems(float elapsedSec);
	void UpdateRemovalFXs(float elapsedSec);
	void UpdateTickTock(float elapsedSec, Camera* pCamera);

	void UpdateEnemy(Enemy*& pEnemy, float elapsedSec);
	void DoEnemyActiveUpdate(Enemy* pEnemy, float elapsedSec);
	void CheckEnemyRemovalConditions(Enemy*& pEnemy);
	void DoEnemyAbsorption(Enemy* pEnemy);
	void DoEnemyKirbyCollision(Enemy* pEnemy);
	void KillEnemy(Enemy* pEnemy, bool addRemovalFX, int scoreReward = 0);

	void UpdateItem(Item*& pItem, float elapsedSec);
	void DoItemActions(Item* pItem);
	void DoItemCollisionEvent(Item* pItem);
	void DoItemRemovalByProjectile(Item* pItem);
	void DoItemInhalationActions(Item*& pItem);
	void DoSolidCollision(Item*& pItem, bool isVerticalCollision, const utils::HitInfo& hitInfo);

	void ClearEnemyVector();
	void ClearObjectVector();
	void ClearFXVector();

	void DeleteEnemies();
	void DeleteItems();
	void DeleteFXs();

	void LoadObjectsFromFile(const std::string& filePath);
	void CreateObject(const std::string& objectName, const Point2f& location);

	void SetEnemyProjectileManagerPointers();

	void DoCameraFunctionality(Camera* pCamera);
	void DoInteractionsWithKirby();
};

