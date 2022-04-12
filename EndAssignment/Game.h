#pragma once
class Kirby;
class Level;
class HUD;
class Camera;
class PowerStar;
class ProjectileManager;
class ObjectManager;
class LevelManager;

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
	bool m_LegacyMode;

	const Window m_Window;

	Kirby* m_pKirby;
	HUD* m_pHUD;
	Camera* m_pCamera;
	ObjectManager* m_pObjectManager;
	LevelManager* m_pLevelManager;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;

	void DrawGeneral() const;
};