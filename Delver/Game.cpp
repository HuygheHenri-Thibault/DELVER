#include "pch.h"
#include "Game.h"
#include "Room.h"

#include "TurretBehaviour.h"
#include "TurretDrawing.h"

Game::Game( const Window& window )
	: m_Window{ window }
	//, BulletManager::GetInstance(){ BulletManager::GetInstance() }
	//, TextureManager::GetInstance(){ TextureManager::GetInstance() }
	//, EnemyManager::GetInstance(){ EnemyManager::GetInstance() }
	, m_Player{ Point2f{window.width / 2, window.height / 2} }
	// , m_TestEnemy{ Actor::ActorData{Point2f{ 0, 0 }, Actor::Dimension{48.f, 48.f, Rectf{0, 0, 48.f, 48.f}}}, Enemy::BehaviourSet{}, 400.f, 1, nullptr }
	// , m_TestEnemy{ Point2f{ 0, 0 }, 400.f, 1, nullptr, 48.f, 48.f }
	, m_MousePos{0, 0}
	, m_Camera{ window.width, window.height, 100.f, 100.f, Point2f{ 0, 0 } }
	, m_Level{ 4, 4, this }
	, m_pCrosshairTexture{nullptr}
	, m_IsPaused{false}
{
	Initialize( );
}
Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_pCrosshairTexture = TextureManager::GetInstance()->GetTexture("Resources/Textures/UI/Cursor_crosshair.png");
	SDL_ShowCursor(SDL_DISABLE);

	//TestBulletManager();
	m_Level.GenerateNextLevel();
	m_Camera.SetLevelBoundaries(m_Level.GetLevelBounds());
	
	InitPlayer();
	m_Player.SetPosition(m_Level.GetPlayerSpawnPoint());
	m_Camera.SetCenterPos(m_Player.GetPosition());

	EnemyManager::GetInstance()->m_pCurrentLevel = &m_Level;



	// Enemy testing // 
	Point2f pos{ m_Level.GetPlayerSpawnPoint() };
	pos.x += 500.f;

	Enemy* newEnemy{ new Enemy{ Actor::ActorData{pos, Actor::Dimension{48.f, 48.f, Rectf{0, 0, 48.f, 48.f}}}, Enemy::BehaviourSet{}, 400.f, 1 } };
	newEnemy->EquipGun(new Gun(0.5f, 400.f, 0.1f, nullptr, BulletType::light));

	Enemy::BehaviourSet turretBehaviour{};
	turretBehaviour.fightingBehaviour = new TurretBehaviour(newEnemy, &m_Player, m_Level);
	turretBehaviour.drawingBehaviour = new TurretDrawing(newEnemy);

	newEnemy->SetBehaviour(turretBehaviour);

	EnemyManager::GetInstance()->AddEnemy(newEnemy);
}
void Game::Cleanup( )
{
	delete BulletManager::GetInstance();
	//BulletManager::GetInstance() = nullptr;

	delete TextureManager::GetInstance();
	//TextureManager::GetInstance() = nullptr;

	delete EnemyManager::GetInstance();
	//EnemyManager::GetInstance() = nullptr;
}

void Game::Update( float elapsedSec )
{
	m_Level.Update(m_Player.GetPosition());
	if (m_IsPaused)
	{
		return;
	}

	BulletManager::GetInstance()->UpdateBullets(elapsedSec, m_Level);
	m_Player.Update(elapsedSec, m_Level, m_MousePos + m_Camera.GetClampDisplacement(m_Player.GetPosition()));

	// m_TestEnemy.Update(elapsedSec, m_Level);
	EnemyManager::GetInstance()->UpdateEnemies(elapsedSec);

	m_Camera.UpdatePos(m_Player.GetPosition());
}

void Game::Draw( ) const
{
	ClearBackground( );

	glPushMatrix();
	m_Camera.Transform();

	m_Level.Draw();

	//m_TestEnemy.Draw();
	EnemyManager::GetInstance()->DrawEnemies();
	m_Player.Draw();

	BulletManager::GetInstance()->DrawBullets();

	// below should be moved to gun
	Gun* gun{ m_Player.GetEquippedGun() };
	//glColor3f(1.f, 1.f, 1.f);
	//utils::DrawLine(gun->GetAimPos(), gun->GetGunPos());
	// up should be moved to gun

	const float crosshairWidth{ m_pCrosshairTexture->GetWidth() };
	const float crosshairHeight{ m_pCrosshairTexture->GetWidth() };
	m_pCrosshairTexture->Draw(Rectf{ gun->GetAimPos().x - crosshairWidth, gun->GetAimPos().y - crosshairHeight, crosshairWidth * 2, crosshairHeight * 2 });

	Room* playerRoom = m_Level.GetRoomAt(m_Player.GetPosition());
	if (playerRoom != nullptr)
	{
		std::vector<std::vector<Point2f>> walls = playerRoom->GetBarriers();
		for (std::vector<Point2f> wall : walls)
		{
			glColor3f(1.f, 0.f, 0.f);
			utils::DrawPolygon(wall);
		}
	}
	
	glPopMatrix();
}

void Game::PauseGame()
{
	m_IsPaused = true;
	SDL_ShowCursor(SDL_ENABLE);
}
void Game::ResumeGame()
{
	m_IsPaused = false;
	SDL_ShowCursor(SDL_DISABLE);
}
void Game::HandleNewLevel()
{
	m_Player.SetPosition(m_Level.GetPlayerSpawnPoint());
	m_Camera.SetCenterPos(m_Player.GetPosition());

	BulletManager::GetInstance()->ClearAll();
	EnemyManager::GetInstance()->ClearAll();

	// spawn new ones for the new level
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}
void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}

}
void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
	m_MousePos.x = float(e.x);
	m_MousePos.y = m_Window.height - float(e.y);
	//m_pGun.UpdateAimPos(m_MousePos);
	
}
void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		//std::cout << " left button " << std::endl;
		m_Player.GetEquippedGun()->StartFiring();
		break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	}
}
void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		//std::cout << " left button " << std::endl;
		m_Player.GetEquippedGun()->StopFiring();
		break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

// Tests
void Game::TestBulletManager()
{
	Bullet* pSomeBullet = BulletManager::GetInstance()->GetBullet(BulletType::light, SpecialEffect::Type::warp);
	BulletManager::GetInstance()->QueueForDestroy(pSomeBullet);
	BulletManager* bm = BulletManager::GetInstance();
	pSomeBullet = bm->GetBullet(BulletType::medium);
	bm->QueueForDestroy(pSomeBullet);
}
void Game::InitPlayer()
{
	m_Player.EquipGun(new Gun(0.5f, 400.f, 0.05f, nullptr, BulletType::light, SpecialEffect::Type::bounce));
}