#include "pch.h"
#include "Game.h"
#include "utils.h"
#include "Kirby.h"
#include "HUD.h"
#include "Camera.h"
#include "ObjectManager.h"
#include "ProjectileManager.h"
#include "LevelManager.h"
#include "PauseScreen.h"
#include "TextureManager.h"
// CLEANED INCLUDES

Game::Game( const Window& window ) 
	: m_Window{ window }
	, m_CurrentLevel{0}
	, m_ParallaxMode{true}
	, m_GameState{GameState::play}
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
	//
	m_pTextureManager = new TextureManager{};
	m_pKirby = new Kirby{};
	m_pHUD = new HUD{};
	m_pPauseScreen = new PauseScreen{m_Window, m_pKirby};
	m_pCamera = new Camera{ m_pKirby->GetLocation(), m_Window.width, m_Window.height, m_pHUD };
	m_pProjectileManager = new ProjectileManager{};
	m_pLevelManager = new LevelManager{ m_pKirby, m_pCamera };
	m_pObjectManager = new ObjectManager{ m_pKirby, m_pLevelManager, m_pProjectileManager };
	// The reason I opted to use a singleton pattern for the object manager is because every game object uses the objectmanager to determine
	// whether they are on screen, but they aren't actually managed by the object manager. An object manager pointer in every game object
	// might thus make another developer think that every game object is managed by this manager, which is not the case (only enemies, items and the boss but
	// I actually find passing along these manager pointers a lot messier than using the singleton pattern
	// Now I have these long trails of pointers being passed along that actually make it unclear where everything is needed
	// These managers are sometimes only needed once within a class and it demands some weird code for my powerups to work such as a method
	// in Actor that allows it to share its projectile manager reference to the powerup. Sometimes these manager objects are
	// also only needed once within a certain class and require a long trail of passing along to make it even work. I find the
	// strict need for encapsulation here more of a limitation than a benefit. The idea that you can also have more than one
	// instance of a manager seems like a bad idea. 
	
	// To fix bootstrap problem with kirby and levelmanager which both need each other, level manager is set after initialization
	m_pKirby->SetLevelManager(m_pLevelManager);
	m_pKirby->SetProjectileManager(m_pProjectileManager);

	m_pLevelManager->LoadLevel("part1");
	m_pKirby->SetLocation(m_pLevelManager->GetCurrentLevel()->GetStartLocation());
}

void Game::Cleanup( )
{
	delete m_pLevelManager;
	delete m_pHUD;
	delete m_pCamera;
	delete m_pKirby;
	delete m_pObjectManager;
	delete m_pProjectileManager;
	delete m_pPauseScreen;
	delete m_pTextureManager;
}

void Game::Update( float elapsedSec )
{
	if (m_GameState == GameState::play)
	{
		UpdatePlay(elapsedSec);
	}
}

void Game::Draw( ) const
{
	ClearBackground( );
	
	switch (m_GameState)
	{
	case GameState::play:
		DrawPlay();
		break;
	case GameState::pause:
		DrawPause();
		break;
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	m_pKirby->ProcessKeyDown(e);

	switch (e.keysym.sym)
	{
	case SDLK_i:
		PrintInfo();
		break;
	case SDLK_ESCAPE:
		TogglePlay();
		break;
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	m_pKirby->ProcessKeyUp(e);

	switch (e.keysym.sym)
	{
	case SDLK_p:
		m_ParallaxMode = !m_ParallaxMode;
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{

}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	m_pPauseScreen->CheckMouseInput(e);
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
	if (m_ParallaxMode)
	{
		m_pLevelManager->DrawLevelParallax();
	}
	else
	{
		m_pLevelManager->DrawLevelLegacy();
	}
}

void Game::PrintInfo() const
{
	std::cout << "CONTROLS:\n\n"
			  << "ESCAPE: pause game\n"
			  << "A/LEFT ARROW: move left, slide left if ducked \n"
			  << "D/RIGHT ARROW: move right, slide right if ducked \n"
			  << "W/UP ARROW: inflate / fly up / jump if bloated\n"
			  << "S/DOWN ARROW: duck \n"
			  << "SPACE: jump\n"
			  << "E: inhale enemies & items / use power / exhale if flying / shoot star if enemy swallowed \n"
			  << "R: discard power \n";
}

void Game::TogglePlay()
{
	if (m_GameState == GameState::play)
	{
		m_GameState = GameState::pause;
	}
	else if (m_GameState == GameState::pause)
	{
		m_GameState = GameState::play;
	}
}

void Game::UpdatePlay(float elapsedSec)
{
	m_pHUD->Update(elapsedSec);
	m_pKirby->Update(elapsedSec);
	m_pCamera->Update(elapsedSec, m_pKirby);
	m_pObjectManager->Update(elapsedSec, m_pCamera);
	m_pProjectileManager->Update(elapsedSec);
}

void Game::DrawPlay() const
{
	// BACKGROUND AREA
	DrawLevel();

	// GAME AREA
	glPushMatrix();
	m_pCamera->Transform();
	m_pObjectManager->Draw();
	m_pKirby->Draw();
	m_pProjectileManager->Draw();
	glPopMatrix();

	// UI AREA
	glPushMatrix();
	m_pCamera->TransformHUD();
	m_pHUD->Draw(m_pKirby);
	glPopMatrix();
}

void Game::DrawPause() const
{
	m_pPauseScreen->Draw();
}
