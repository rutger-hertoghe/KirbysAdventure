#pragma once

class Level;
class Camera;
class Kirby;
class ObjectManager;

class LevelManager final
{
public:
	LevelManager(Kirby* pKirby, Camera* pCamera);
	~LevelManager();

	static LevelManager* GetLevelMngr();
	static Level* GetCurrentLevel();

	void LoadLevel(std::string levelName);
	void DrawLevelLegacy() const;
	void DrawLevelParallax() const;

private:
	static LevelManager* m_pLevelManager;
	Level* m_pCurrentLevel;
	Camera* m_pCamera;
	Kirby* m_pKirby;

	std::vector<Level*> m_pLevels;

	void Initialize();
};

