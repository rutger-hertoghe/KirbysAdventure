#include "pch.h"
#include "Game.h"
#include "utils.h"
#include "PowerUp.h"
#include "FirePower.h"
#include "SparkPower.h"
#include "GameObject.h"
#include "PowerStar.h"
#include "Kirby.h"
#include "Level.h"
#include "HUD.h"
#include "Camera.h"
#include <iostream>

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
	m_pKirby = new Kirby();
	m_pTestStar = new PowerStar(Rectf{ 30.f, 30.f, 16.f, 16.f });
	m_pTestStar->SetPowerUp(new FirePower{});
	m_pTestStar->TransferPowerUp(m_pKirby);
	m_pLevel = new Level("resources/backgrounds/part1_full.png");
	m_pHUD = new HUD();

	m_pCamera = new Camera(m_pKirby->GetLocation(), m_Window.width, m_Window.height, *m_pLevel, *m_pHUD);
}

void Game::Cleanup( )
{
	delete m_pKirby;
	delete m_pLevel;
	delete m_pTestStar;
	delete m_pHUD;
	delete m_pCamera;
}

void Game::Update( float elapsedSec )
{
	m_pKirby->Update(elapsedSec, *m_pLevel);
	m_pHUD->Update(elapsedSec);
	m_pCamera->Update(m_pKirby->GetLocation().x, m_pKirby->GetLocation().y);
}

void Game::Draw( ) const
{
	ClearBackground( );
	// GAME AREA
	glPushMatrix();
	m_pCamera->Transform();

	m_pLevel->Draw();
	m_pKirby->Draw();
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
		if (m_pKirby->HasPower()) m_pKirby->TransferPowerUp(m_pTestStar);
		else if (m_pTestStar->HasPower()) m_pTestStar->TransferPowerUp(m_pKirby);
		m_pKirby->EnforceState();
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
