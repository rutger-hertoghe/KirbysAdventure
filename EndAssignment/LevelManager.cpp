#include "pch.h"
#include "LevelManager.h"
#include "Level.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "Kirby.h"

// LevelManager* LevelManager::m_pLevelManager{ nullptr };

LevelManager::LevelManager(Kirby* pKirby, Camera* pCamera)
	: m_pKirby(pKirby)
	, m_pCamera{ pCamera }
{

	Initialize();
}

LevelManager::~LevelManager()
{
	for (Level* pLevel : m_pLevels)
	{
		delete pLevel;
	}
}

void LevelManager::Initialize()
{
	// m_pLevels.push_back(new Level{ "testlevel", "resources/music/stagemusic1.mp3" });
	m_pLevels.push_back(new Level{ "part1", "resources/music/stagemusic1.mp3" });
	m_pLevels.push_back(new Level{ "part2", "resources/music/stagemusic1.mp3" });
	m_pLevels.push_back(new Level{ "part3", "resources/music/stagemusic1.mp3" });
	// m_pLevels.push_back(new Level{ "part4", "resources/music/stagemusic1.mp3" });
	// m_pLevels.push_back(new Level{ "part5", "resources/music/stagemusic1.mp3" });
	// m_pLevels.push_back(new Level{ "part6", "resources/music/stagemusic1.mp3" });
}

void LevelManager::LoadLevel(std::string levelName)
{	
	ObjectManager* pObjectMngr{ ObjectManager::GetObjectMngr() };;
	if (levelName == "")
	{
		std::cout << "CONNECTED LEVEL NOT FOUND\n";
		return;
	}

	if (pObjectMngr) pObjectMngr->Clear();

	for (Level*& pLevel : m_pLevels)
	{
		if (pLevel->GetName() == levelName)
		{
			m_pCurrentLevel = pLevel;
		}
	}

	m_pCamera->UpdateBoundaries(m_pCurrentLevel->GetBoundaries());
	if (pObjectMngr) pObjectMngr->LoadObjectsByLevelName(levelName);
}

Level* LevelManager::GetCurrentLevel()
{
	return m_pCurrentLevel;
}

void LevelManager::DrawLevelLegacy() const
{
	glPushMatrix();
	m_pCamera->Transform();
	m_pCurrentLevel->DrawFull();
	glPopMatrix();
}

void LevelManager::DrawLevelParallax() const
{
	const float farBackgroundScalar{ 8.f };
	const float backgroundScalar{ 4.f };
	const float midgroundScalar{ 2.f };

	glPushMatrix();
	m_pCamera->Transform(farBackgroundScalar);
	m_pCurrentLevel->DrawFarBackGround();
	glPopMatrix();

	glPushMatrix();
	m_pCamera->Transform(backgroundScalar);
	m_pCurrentLevel->DrawBackGround();
	glPopMatrix();

	glPushMatrix();
	m_pCamera->Transform(midgroundScalar);
	m_pCurrentLevel->DrawMidGround();
	glPopMatrix();

	glPushMatrix();
	m_pCamera->Transform();
	m_pCurrentLevel->DrawForeGround(); // If no scalar is supplied, value defaults to 1.f
	glPopMatrix();
}