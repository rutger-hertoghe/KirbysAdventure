#include "pch.h"
#include "LevelManager.h"
#include "Level.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "Kirby.h"
#include "SoundStream.h"

LevelManager::LevelManager(Kirby* pKirby, Camera* pCamera)
	: m_pKirby(pKirby)
	, m_pCamera{ pCamera }
	, m_pMusic{ new SoundStream{"resources/music/stagemusic1.mp3"} }
	, m_pCurrentLevel{nullptr}
{

	Initialize();
}

LevelManager::~LevelManager()
{
	for (auto pLevel : m_pLevels)
	{
		delete pLevel.second;
	}
	delete m_pMusic;
}

void LevelManager::Initialize()
{
	AddLevel("part1", m_pMusic);
	AddLevel("part2", m_pMusic);
	AddLevel("part3", m_pMusic);
}

void LevelManager::AddLevel(const std::string& levelName, SoundStream* music)
{
	m_pLevels.insert(std::pair<std::string, Level*>(levelName, new Level{ levelName, music }));
}

void LevelManager::LoadLevel(const std::string& levelName)
{	
	ObjectManager* pObjectMngr{ ObjectManager::GetObjectMngr() };;
	if (levelName == "")
	{
		std::cout << "CONNECTED LEVEL NOT FOUND\n";
		return;
	}

	if (pObjectMngr) pObjectMngr->Clear();

	SoundStream* pCurrentSong{};
	if (m_pCurrentLevel)
	{
		pCurrentSong = m_pCurrentLevel->GetLevelMusic();
	}

	m_pCurrentLevel = m_pLevels[levelName];

	m_pCamera->UpdateBoundaries(m_pCurrentLevel->GetBoundaries());

	if (pObjectMngr) pObjectMngr->LoadObjectsByLevelName(levelName);

	if (pCurrentSong != m_pCurrentLevel->GetLevelMusic() || !SoundStream::IsPlaying())
	{
		SoundStream::Stop();
		m_pCurrentLevel->PlayMusic();
	}
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