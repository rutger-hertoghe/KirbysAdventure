#pragma once
#include <unordered_map>

class Level;
class Camera;
class Kirby;
class ObjectManager;
class SoundStream;

class LevelManager final
{
public:
	explicit LevelManager(Kirby* pKirby, Camera* pCamera);
	LevelManager& operator=(const LevelManager& other) = delete;
	LevelManager(const LevelManager& other) = delete;
	LevelManager& operator=(LevelManager&& other) = delete;
	LevelManager(LevelManager&& other) = delete;
	~LevelManager();

	Level* GetCurrentLevel();

	void LoadLevel(const std::string& levelName);
	void DrawLevelLegacy() const;
	void DrawLevelParallax() const;

private:
	Level* m_pCurrentLevel;
	Camera* m_pCamera;
	Kirby* m_pKirby;
	SoundStream* m_pMusic;

	std::unordered_map<std::string, Level*> m_pLevels;

	void Initialize();
	void AddLevel(const std::string& levelName, SoundStream* music);
};

