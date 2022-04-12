#include "pch.h"
#include "LevelManager.h"
#include "Level.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "Kirby.h"

LevelManager::LevelManager(Kirby* pKirby, ObjectManager* pObjectMngr)
	: m_pKirby(pKirby)
	, m_pObjectManager{pObjectMngr}
	, m_pCamera{nullptr}
{
	Initialize();
	m_pKirby->SetLevelManager(this);
	m_pCurrentLevel = m_pLevels[0];
	m_pKirby->SetLocation(m_pCurrentLevel->GetStartLocation());
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
	m_pLevels.push_back(new Level{ "part1", "resources/music/stagemusic1.mp3" });
	m_pLevels.push_back(new Level{ "part2", "resources/music/stagemusic1.mp3" });
	m_pLevels.push_back(new Level{ "part3", "resources/music/stagemusic1.mp3" });
	m_pLevels.push_back(new Level{ "part4", "resources/music/stagemusic1.mp3" });
	m_pLevels.push_back(new Level{ "part5", "resources/music/stagemusic1.mp3" });
	m_pLevels.push_back(new Level{ "part6", "resources/music/stagemusic1.mp3" });
}

void LevelManager::LoadLevel(std::string levelName)
{
	if (levelName == "")
	{
		return;
	}
	m_pObjectManager->ClearEnemyVector();

	for (Level*& pLevel : m_pLevels)
	{
		if (pLevel->GetName() == levelName)
		{
			m_pCurrentLevel = pLevel;
		}
	}

	m_pCamera->UpdateBoundaries(m_pCurrentLevel->GetBoundaries());
	m_pKirby->SetCurrentLevel(m_pCurrentLevel);
	m_pObjectManager->LoadEnemiesByLevelName(levelName);

	m_pObjectManager->SetLevelPointers(m_pCurrentLevel);
}

Level* LevelManager::GetCurrentLevel() const
{
	return m_pCurrentLevel;
}

void LevelManager::LinkCamera(Camera* pCamera)
{
	m_pCamera = pCamera;
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
	glPushMatrix();
	m_pCamera->Transform(8.f);
	m_pCurrentLevel->DrawFarBackGround();
	glPopMatrix();

	glPushMatrix();
	m_pCamera->Transform(4.f);
	m_pCurrentLevel->DrawBackGround();
	glPopMatrix();

	glPushMatrix();
	m_pCamera->Transform(2.f);
	m_pCurrentLevel->DrawMidGround();
	glPopMatrix();

	glPushMatrix();
	m_pCamera->Transform();
	m_pCurrentLevel->DrawForeGround();
	glPopMatrix();
}