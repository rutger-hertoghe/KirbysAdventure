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
#include "WaddleDee.h"
#include "PowerStar.h"
#include "EnemyManager.h"

Game::Game( const Window& window ) 
	:m_Window{ window }
{
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_pKirby = new Kirby{};
	m_pCurrentLevel = new Level("resources/backgrounds/part1_full.png", "resources/music/stagemusic1.mp3");
	m_pProjectileManager = new ProjectileManager{};
	m_pHUD = new HUD{};
	m_pCamera = new Camera{ m_pKirby->GetLocation(), m_Window.width, m_Window.height, *m_pCurrentLevel, *m_pHUD };
	m_pEnemyManager = new EnemyManager{};
	m_pTestStar = new PowerStar{ Point2f(100.f, 100.f) };

	m_pEnemyManager->LinkKirby(m_pKirby);
	m_pEnemyManager->LinkProjectileManager(m_pProjectileManager);
	m_pEnemyManager->SupplyViewportDimensions(m_pCamera->GetViewDimensions());
	m_pEnemyManager->LinkLevel(m_pCurrentLevel);

	m_pKirby->SetCurrentLevel(m_pCurrentLevel);
	m_pKirby->SetLocation(m_pCurrentLevel->GetStartLocation());
	m_pTestStar->SetCurrentLevel(m_pCurrentLevel);
	m_pKirby->SetProjectileManager(m_pProjectileManager);
}

void Game::Cleanup( )
{
	delete m_pCurrentLevel;
	delete m_pHUD;
	delete m_pCamera;
	delete m_pKirby;
	delete m_pProjectileManager;
	delete m_pTestStar;
	delete m_pEnemyManager;
}

void Game::Update( float elapsedSec )
{
	m_pHUD->Update(elapsedSec);
	m_pKirby->Update(elapsedSec);
	m_pCamera->Update(m_pKirby->GetLocation());
	m_pProjectileManager->Update(elapsedSec);
	m_pTestStar->Update(elapsedSec);
	m_pEnemyManager->Update(elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground( );
	// GAME AREA
	glPushMatrix();
	m_pCamera->Transform();
	m_pCurrentLevel->Draw();
	m_pKirby->Draw();
	m_pProjectileManager->Draw();
	m_pEnemyManager->Draw();
	m_pTestStar->Draw();
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
	switch (e.keysym.sym)
	{
	case SDLK_a:
		m_pKirby->SetPowerUp(new FirePower{}, m_pProjectileManager);
		break;
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	m_pKirby->ProcessKeyUp(e);
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

