#pragma once

#include "BulletManager.h"
#include "TextureManager.h"
#include "EnemyManager.h"
#include "ItemManager.h"
#include "Gun.h"
#include "Player.h"
#include "Camera.h"
#include "Level.h"
#include "Enemy.h"
#include "EnemyFactory.h"

#include "Menu.h"

#include <vector>

class Game
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

	void PauseGame();
	void ResumeGame();
	void HandleOldLevel();
	void HandleNewLevel();

	EnemyFactory* GetEnemyFactory();

	void ResetGame();
	void PrintControls();

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	const Window m_Window;
	//BulletManager* m_pBulletManager;
	//TextureManager* m_pTextureManager;
	//EnemyManager* m_pEnemyManager;

	Player m_Player;

	Point2f m_MousePos;
	Texture* m_pCrosshairTexture;

	Camera m_Camera;
	Level m_Level;

	bool m_IsPaused;

	EnemyFactory m_EnemyFactory;

	Menu m_Menu;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;

	void TestBulletManager();
	void InitPlayer();
};
