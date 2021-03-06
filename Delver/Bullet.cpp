#include "pch.h"
#include "Bullet.h"

#include "SpecialEffect.h"
#include "Texture.h"
#include "Gun.h"
#include "BulletManager.h"
#include "Enemy.h"
#include "EnemyManager.h"

Bullet::Bullet(BulletType type)
	: m_pBulletTexture{ nullptr }
	, m_Type{ type }
	, m_Velocity{0,0}
	, m_Position{0,0}
	, m_pSpecialEffect{nullptr}
	//, m_pGunWhichFired{nullptr}
	, m_IsGoingToBeDestroyed{ false }
	, m_FiredByType{ Actor::Type::enemy }
{
}
Bullet::~Bullet()
{
	if (m_pSpecialEffect != nullptr)
	{
		delete m_pSpecialEffect;
		//m_pSpecialEffect = nullptr;
		//switch (m_pSpecialEffect->GetType())
		//{
		//case SpecialEffect::Type::bounce:
		//case SpecialEffect::Type::warp:

		//	break;
		//}
	}
}

void Bullet::SetType(const BulletType& type)
{
	m_Type = type;
}
void Bullet::SetSpecialEffect(SpecialEffect* specialEffect)
{
	m_pSpecialEffect = specialEffect;
}
void Bullet::SetPosition(const Point2f& position)
{
	m_Position = position;
}
void Bullet::SetVelocity(const Vector2f& velocity)
{
	m_Velocity = velocity;
}
void Bullet::SetTexture(Texture* texture)
{
	m_pBulletTexture = texture;
	m_Hitbox = Rectf{ 0,0, texture->GetWidth(), texture->GetHeight() };
}
void Bullet::SetFireOrigin(Gun* originGun)
{
	//m_pGunWhichFired = originGun;
	if (originGun == nullptr || originGun->GetHolder() == nullptr)
	{
		m_FiredByType = Actor::Type::enemy;
	}
	else
	{
		m_FiredByType = originGun->GetHolder()->m_Type;
	}	
}

Point2f Bullet::GetPosition() const
{
	return m_Position;
}
Vector2f Bullet::GetVelocity() const
{
	return m_Velocity;
}
bool Bullet::IsGoingToBeDestroyed() const
{
	return m_IsGoingToBeDestroyed;
}

void Bullet::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& wallsVector, Player& player)
{
	bool hasHitAWall{ false };
	for (const std::vector<Point2f>& wall : wallsVector)
	{
		utils::HitInfo hitInfo{};
		if (CheckCollision(wall, hitInfo, elapsedSec)) // check if hit walls
		{
			HandleCollision(hitInfo, elapsedSec);
			hasHitAWall = true;
		}
	}
	if (!hasHitAWall)
	{
		m_Position += m_Velocity * elapsedSec;
	}

	// TODO: check colisions with non-walls and such
	EnemyManager::GetInstance()->CheckCollision(this);
	//bool FiredByEnemy{ m_pGunWhichFired == nullptr || m_pGunWhichFired->GetHolder() == nullptr || m_pGunWhichFired->GetHolder()->m_Type == Actor::Type::enemy };
	if (m_FiredByType == Actor::Type::enemy && player.isPointInCollisionRect(m_Position))
	{
		OnHit(player);
	}
}
void Bullet::Draw() const
{
	// might need to get rotation and such first
	// probably most easy to translate
	// but best to store rotation & scale matricies in variables than recalculating each frame
	if (m_IsGoingToBeDestroyed)
	{
		return;
	}

	if (m_pBulletTexture != nullptr)
	{
		m_pBulletTexture->Draw();
	}
	else
	{
		glColor3f(1.f, 0.f, 0.f);
		utils::FillCircle(Circlef{ m_Position, 3.f });
	}
}

void Bullet::OnHit(Enemy* enemyHit)
{
	if (m_FiredByType == Actor::Type::enemy)
	{
		return;
	}
	if (enemyHit->IsDead())
	{
		return;
	}
	// TODO: Apply more stuff here depending one defence and such of the Enemy and damage of the bullet
	enemyHit->m_CurrentHitpoints--;
	// TODO: Make a special effect on hit and call it here too.
	BulletManager::GetInstance()->QueueForDestroy(this);
	m_IsGoingToBeDestroyed = true;
}
void Bullet::OnHit(Player& playerHit)
{
	playerHit.m_CurrentHp--;
	playerHit.GotHit();
	BulletManager::GetInstance()->QueueForDestroy(this);
	m_IsGoingToBeDestroyed = true;
}

bool Bullet::CheckCollision(const std::vector<Point2f>& other, utils::HitInfo& hitInfo, float elapsedSec) const
{
	Point2f nextPos{ m_Position + m_Velocity * elapsedSec };
	bool res{ utils::Raycast(other, m_Position, nextPos, hitInfo) };
	return res;
}
void Bullet::HandleCollision(const utils::HitInfo& hitInfo, float elapsedSec)
{


	if (m_pSpecialEffect == nullptr || m_pSpecialEffect->GetType() == SpecialEffect::Type::none)
	{
		BulletManager::GetInstance()->QueueForDestroy(this);
		m_IsGoingToBeDestroyed = true;
	}
	else
	{
		m_pSpecialEffect->ApplySpecialEffect(this, hitInfo, elapsedSec);
		if (m_pSpecialEffect->IsSpecialEffectDone())
		{
			BulletManager::GetInstance()->QueueForDestroy(this);
			m_IsGoingToBeDestroyed = true;
		}
		// handle collison with wall for special bullets here 
			// - bounce will need to bounce
			// - time warp goes back 1sec
			// - explosive, well explodes...
	}
}