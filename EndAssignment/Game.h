#pragma once
class Kirby;
class Level;
class HUD;
class Camera;
class PowerStar;
class Projectile;
class ProjectileManager;
class Enemy;
class Actor;

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
	const Window m_Window;
	Kirby* m_pKirby;
	Level* m_pCurrentLevel;
	HUD* m_pHUD;
	Camera* m_pCamera;
	ProjectileManager* m_pProjectileManager;
	Enemy* m_pTestEnemy;
	PowerStar* m_pTestStar;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;

	void UpdateEnemy(Enemy*& pEnemy, float elapsedSec);
	void CheckEnemyRemovalConditions(Enemy*& pEnemy);
};