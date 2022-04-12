#pragma once

class Level;
class Camera;
class Kirby;
class ObjectManager;

class LevelManager final
{
public:
	LevelManager(Kirby* pKirby, ObjectManager* pObjectMngr);
	~LevelManager();

	void LoadLevel(std::string levelName);
	Level* GetCurrentLevel() const;
	void LinkCamera(Camera* pCamera);
	void DrawLevelLegacy() const;
	void DrawLevelParallax() const;

private:
	Level* m_pCurrentLevel;
	Camera* m_pCamera;
	Kirby* m_pKirby;
	ObjectManager* m_pObjectManager;

	std::vector<Level*> m_pLevels;

	void Initialize();
};

