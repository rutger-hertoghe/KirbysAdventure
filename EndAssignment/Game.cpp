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
	m_pCurrentLevel = new Level("resources/backgrounds/part1_full.png", "resources/music/stagemusic1.mp3");
	m_pProjectileManager = new ProjectileManager{};
	m_pHUD = new HUD();
	m_pCamera = new Camera( m_pKirby->GetLocation(), m_Window.width, m_Window.height, *m_pCurrentLevel, *m_pHUD);
	m_pTestEnemy = new WaddleDee{ Point2f(200.f, 100.f) };
	m_pTestStar = new PowerStar{ Point2f(100.f, 100.f) };

	m_pKirby->SetCurrentLevel(m_pCurrentLevel);
	m_pKirby->SetLocation(m_pCurrentLevel->GetStartLocation());
	m_pTestEnemy->SetCurrentLevel(m_pCurrentLevel);
	m_pTestEnemy->SetPowerUp(new FirePower{}, m_pProjectileManager);
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
	if (m_pTestEnemy) delete m_pTestEnemy;
}

void Game::Update( float elapsedSec )
{
	m_pHUD->Update(elapsedSec);
	m_pKirby->Update(elapsedSec);
	m_pCamera->Update(m_pKirby->GetLocation());
	m_pProjectileManager->Update(elapsedSec);
	m_pTestStar->Update(elapsedSec);

	UpdateEnemy(m_pTestEnemy, elapsedSec);
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
	if (m_pTestEnemy->IsActive())
	{
		m_pTestEnemy->Draw();
	}
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
		m_pKirby->SetPowerUp(new SparkPower{}, m_pProjectileManager);
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

void Game::UpdateEnemy(Enemy*& pEnemy, float elapsedSec)
{
	if (pEnemy->IsActive())
	{
		pEnemy->Update(elapsedSec);

		if (pEnemy->IsInhalable())
		{
			pEnemy->ToggleBeingInhaled(m_pKirby->GetInhalationZone());
		}

		if (pEnemy->IsBeingInhaled())
		{
			pEnemy->SetInhalationVelocities(m_pKirby->GetRect());
		}
		
		CheckEnemyRemovalConditions(pEnemy);
	}
	else if (pEnemy->HasBeenOffScreen()
		&& abs(pEnemy->GetRect().left - m_pKirby->GetRect().left) < m_pCamera->GetViewDimensions().x * 2 / 3
		&& abs(pEnemy->GetRect().bottom - m_pKirby->GetRect().bottom) < m_pCamera->GetViewDimensions().y * 2 / 3)
	{
		pEnemy->SetActivity(true);
	}
	else if (abs(pEnemy->GetRect().left - m_pKirby->GetRect().left) > m_pCamera->GetViewDimensions().x * 2 / 3
		|| abs(pEnemy->GetRect().bottom - m_pKirby->GetRect().bottom) > m_pCamera->GetViewDimensions().y * 2 / 3)
	{
		pEnemy->SetOffScreen(true);
	}
}

void Game::CheckEnemyRemovalConditions(Enemy*& pEnemy)
{
	if (m_pProjectileManager->ProjectileHasHit(pEnemy))
	{
		pEnemy->Reset();
	}
	else if (pEnemy->CheckCollision(m_pKirby->GetRect()))
	{
		if (m_pKirby->IsInhaling())
		{
			m_pKirby->SetBloated();
			if (pEnemy->GetPowerUp())
			{
				pEnemy->TransferPowerUp(m_pKirby);
			}
		}
		else
		{
			m_pKirby->DecrementHealth();
		}
		pEnemy->Reset();
	}
}
