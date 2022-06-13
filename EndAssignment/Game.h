#pragma once
#include "SoundFXManager.h"
class Kirby;
class HUD;
class Camera;
class ProjectileManager;
class ObjectManager;
class LevelManager;
class PauseScreen;

enum class GameState
{
	play,
	pause
};

class Game final
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	int m_CurrentLevel;
	bool m_ParallaxMode;

	const Window m_Window;

	GameState m_GameState;

	// MrTickTock* m_pMrTickTock;
	Kirby* m_pKirby;
	HUD* m_pHUD;
	PauseScreen* m_pPauseScreen;
	Camera* m_pCamera;
	ObjectManager* m_pObjectManager;
	ProjectileManager* m_pProjectileManager;
	LevelManager* m_pLevelManager;
	SoundFXManager m_SoundFXManager;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;

	void DrawLevel() const;
	void PrintInfo() const;
	void TogglePlay();
	void UpdatePlay(float elapsedSec);
	void DrawPlay() const;
	void DrawPause() const;
};