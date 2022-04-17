#pragma once

class Level;
class Camera;
class Kirby;
class ObjectManager;

class LevelManager final
{
public:
	LevelManager(Kirby* pKirby, ObjectManager* pObjectMngr, Camera* pCamera);
	~LevelManager();

	void LoadLevel(std::string levelName);
	static Level* GetCurrentLevel();
	void DrawLevelLegacy() const;
	void DrawLevelParallax() const;

private:
	static Level* m_pCurrentLevel;
	Camera* m_pCamera;
	Kirby* m_pKirby;
	ObjectManager* m_pObjectManager;

	std::vector<Level*> m_pLevels;

	void Initialize();
};

