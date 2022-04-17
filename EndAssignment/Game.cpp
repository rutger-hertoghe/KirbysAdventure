#include "pch.h"
#include "Game.h"
#include "utils.h"
#include "Kirby.h"
#include "HUD.h"
#include "Camera.h"
#include "Projectile.h"
#include "PowerStar.h"
#include "ObjectManager.h"
#include "ProjectileManager.h"
#include "LevelManager.h"

#include "MrTickTock.h"

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
	// TODO: Implement boss enemy
	m_pKirby = new Kirby{};
	m_pHUD = new HUD{};
	m_pObjectManager = new ObjectManager{m_pKirby};
	m_pObjectManager = new ObjectManager{m_pKirby};
	m_pProjectileManager = new ProjectileManager{};
	m_pCamera = new Camera{ m_pKirby->GetLocation(), m_Window.width, m_Window.height, m_pHUD, m_pObjectManager};
	m_pLevelManager = new LevelManager{ m_pKirby, m_pObjectManager, m_pCamera };
	
	// m_pMrTickTock = new MrTickTock{ Point2f{312.f, 40.f} };
}

void Game::Cleanup( )
{
	delete m_pLevelManager;
	delete m_pHUD;
	delete m_pCamera;
	delete m_pKirby;
	delete m_pObjectManager;
	delete m_pProjectileManager;

	// delete m_pMrTickTock;
}

void Game::Update( float elapsedSec )
{
	m_pHUD->Update(elapsedSec);
	m_pKirby->Update(elapsedSec);
	m_pCamera->Update(elapsedSec, m_pKirby);
	m_pObjectManager->Update(elapsedSec, m_pCamera->GetVisibleArea());
	m_pProjectileManager->Update(elapsedSec);
	
	// m_pMrTickTock->Update(elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground( );
	
	// BACKGROUND AREA
	DrawLevel();

	// GAME AREA
	glPushMatrix();
	m_pCamera->Transform();
	m_pKirby->Draw();
	m_pObjectManager->Draw();
	m_pProjectileManager->Draw();
	
	// m_pMrTickTock->Draw();
	glPopMatrix();

	// UI AREA
	glPushMatrix();
	m_pCamera->TransformHUD();
	m_pHUD->Draw(m_pKirby);
	glPopMatrix();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	m_pKirby->ProcessKeyDown(e);
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	m_pKirby->ProcessKeyUp(e);

	switch (e.keysym.sym)
	{
	case SDLK_p:
		m_LegacyMode = !m_LegacyMode;
		break;
	
	case SDLK_t:
		// m_pObjectManager->ResetEnemies();
		break;
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

void Game::DrawLevel() const
{
	if (m_LegacyMode)
	{
		m_pLevelManager->DrawLevelLegacy();
	}
	else
	{
		m_pLevelManager->DrawLevelParallax();
	}
}


