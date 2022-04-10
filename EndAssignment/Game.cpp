#include "pch.h"
#include "Game.h"
#include "utils.h"
#include "PowerUp.h"
#include "FirePower.h"
#include "SparkPower.h"
#include "GameObject.h"
#include "Kirby.h"
#include "Level.h"
#include "HUD.h"
#include "Camera.h"
#include <iostream>
#include "Fireball.h"
#include "Projectile.h"
#include "ProjectileManager.h"
#include "PowerStar.h"
#include "ObjectManager.h"

Game::Game( const Window& window ) 
	: m_Window{ window }
	, m_CurrentLevel{0}
	, m_LegacyMode{true}
{
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	InitializeLevels();

	m_pKirby = new Kirby{};
	m_pHUD = new HUD{};
	m_pObjectManager = new ObjectManager{};
	m_pCurrentLevel = m_pLevels[0];

	m_pCamera = new Camera{ m_pKirby->GetLocation(), m_Window.width, m_Window.height, *m_pCurrentLevel, *m_pHUD };

	m_pObjectManager->LinkKirby(m_pKirby);
	m_pObjectManager->SupplyViewportDimensions(m_pCamera->GetViewDimensions());

	LoadLevel("part1");
}

void Game::Cleanup( )
{
	for (Level* pLevel : m_pLevels)
	{
		delete pLevel;
	}
	delete m_pHUD;
	delete m_pCamera;
	delete m_pKirby;
	delete m_pObjectManager;
}

void Game::Update( float elapsedSec )
{
	m_pHUD->Update(elapsedSec);
	m_pKirby->Update(elapsedSec);
	m_pCamera->Update(m_pKirby->GetLocation(), elapsedSec);
	m_pObjectManager->Update(elapsedSec, m_pCamera->GetLocation());
}

void Game::Draw( ) const
{
	ClearBackground( );
	// GAME AREA
	if (m_LegacyMode)
	{
		DrawLevelLegacy();
	}
	else
	{
		DrawLevelParallax();
	}

	DrawGeneral();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	m_pKirby->ProcessKeyDown(e);
	switch (e.keysym.sym)
	{
	case SDLK_RETURN:
		++m_CurrentLevel %= 6;
		LoadLevel(m_pLevels[m_CurrentLevel]->GetName());
		break;
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	m_pKirby->ProcessKeyUp(e);

	switch (e.keysym.sym)
	{
	case SDLK_p:
		m_LegacyMode = !m_LegacyMode;
		break;
	case SDLK_a:
		if (m_pKirby->HasPowerUp())
		{
			PowerStar* pPowerStar{ new PowerStar{m_pKirby->GetLocation()} };
			pPowerStar->SetCurrentLevel(m_pCurrentLevel);
			pPowerStar->SetDirection(-m_pKirby->GetDirection());
			m_pKirby->TransferPowerUp(pPowerStar);
			m_pObjectManager->AddItem(pPowerStar);
		}
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{

}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::InitializeLevels()
{
	m_pLevels.push_back(new Level{ "part1", "resources/music/stagemusic1.mp3" });
	m_pLevels.push_back(new Level{ "part2", "resources/music/stagemusic1.mp3" });
	m_pLevels.push_back(new Level{ "part3", "resources/music/stagemusic1.mp3" });
	m_pLevels.push_back(new Level{ "part4", "resources/music/stagemusic1.mp3" });
	m_pLevels.push_back(new Level{ "part5", "resources/music/stagemusic1.mp3" });
	m_pLevels.push_back(new Level{ "part6", "resources/music/stagemusic1.mp3" });
}

void Game::LoadLevel(std::string levelName)
{
	m_pObjectManager->ClearEnemyVector();

	for (Level*& pLevel : m_pLevels)
	{
		if (pLevel->GetName() == levelName)
		{
			m_pCurrentLevel = pLevel;
		}
	}

	m_pCamera->UpdateBoundaries(*m_pCurrentLevel);
	m_pKirby->SetCurrentLevel(m_pCurrentLevel);
	m_pObjectManager->LoadEnemiesByLevelName(levelName);
	
	m_pObjectManager->SetLevelPointers(m_pCurrentLevel);
	m_pKirby->SetLocation(m_pCurrentLevel->GetStartLocation());
}

void Game::DrawLevelLegacy() const
{
	glPushMatrix();
	m_pCamera->Transform();
	m_pCurrentLevel->DrawFull();
	glPopMatrix();
}

void Game::DrawLevelParallax() const
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

void Game::DrawGeneral() const
{
	glPushMatrix();
	m_pCamera->Transform();
	m_pKirby->Draw();
	m_pObjectManager->Draw();
	glPopMatrix();

	// UI AREA
	glPushMatrix();
	m_pCamera->TransformHUD();
	m_pHUD->Draw(m_pKirby);
	glPopMatrix();
}


