#include "pch.h"
#include "Enemy.h"
#include "Texture.h"

#include "StationaryBeheviour.h"
#include "PeacefulBehaviour.h"
#include "DrawingBehaviour.h"

#include "Gun.h"

Enemy::Enemy(const ActorData& actorData, const BehaviourSet& behaviours, float detectionRange, int hitPoints, Gun* gun)
	: Actor(actorData)
	, m_DetectionRange{ detectionRange }
	, m_MaxHitPoints{ hitPoints }
	, m_CurrentHitpoints{ hitPoints }
	, m_pMovementBehavior{ behaviours.movementBehaviour }
	, m_pFightingBehaviour{ behaviours.fightingBehaviour }
	, m_pDrawingBehaviour{ behaviours.drawingBehaviour }
	, m_pEquippedGun{ gun }
	, m_State{ State::idle }
	, m_pTarget{ nullptr }
{
	Initialize();
}

Enemy::Enemy(const Point2f& pos, float detectionRange, int hitPoints, Texture* texture, float width, float height, Gun* gun, 
	MovementBehaviour* movementBehaviour, FightingBehaviour* fightingBehaviour, DrawingBehaviour* drawingBehaviour)
	: Actor(pos, Actor::Type::enemy, texture, width, height)
	, m_DetectionRange{ detectionRange }
	, m_MaxHitPoints{ hitPoints }
	, m_CurrentHitpoints{ hitPoints }
	, m_pMovementBehavior{ movementBehaviour }
	, m_pFightingBehaviour{ fightingBehaviour }
	, m_pDrawingBehaviour{ drawingBehaviour }
	, m_pEquippedGun{ gun }
	, m_State{ State::idle }
	, m_pTarget{ nullptr }
{
	Initialize();
}
Enemy::~Enemy()
{
	if (m_pMovementBehavior != nullptr)
	{
		delete m_pMovementBehavior;
		m_pMovementBehavior = nullptr;
	}
	if (m_pFightingBehaviour != nullptr)
	{
		delete m_pFightingBehaviour;
		m_pFightingBehaviour = nullptr;
	}
	if (m_pDrawingBehaviour != nullptr)
	{
		delete m_pDrawingBehaviour;
		m_pDrawingBehaviour = nullptr;
	}

	if (m_pEquippedGun != nullptr)
	{
		delete m_pEquippedGun;
		m_pEquippedGun = nullptr;
	}
}

void Enemy::Initialize()
{
	if (m_pEquippedGun != nullptr)
	{
		m_pEquippedGun->SetHolder(this);
		m_pEquippedGun->UpdateGunPos(m_Position);
	}
}

void Enemy::Update(float elapsedSec, const Level& level)
{
	if (m_pMovementBehavior != nullptr)
	{
		m_pMovementBehavior->Update(elapsedSec);
	}
	if (m_pFightingBehaviour != nullptr)
	{
		m_pFightingBehaviour->Update(elapsedSec);
	}
	if (m_pDrawingBehaviour != nullptr)
	{
		m_pDrawingBehaviour->Update(elapsedSec);
	}


	if (m_pEquippedGun != nullptr)
	{
		m_pEquippedGun->UpdateGun(elapsedSec);
	}

	Actor::Update(elapsedSec, level);
}
void Enemy::Draw() const
{
	if (m_pDrawingBehaviour != nullptr)
	{
		m_pDrawingBehaviour->Draw();
	}
	else
	{
		Actor::Draw();
	}
}

bool Enemy::IsTargetInRange(const Actor& target) const
{
	return utils::GetDistance(m_Position, target.GetPosition()) < m_DetectionRange;
}

bool Enemy::IsDead() const
{
	return m_CurrentHitpoints <= 0;
}
void Enemy::HandleDeath()
{
	// TODO
}

void Enemy::SetMovementBehaviour(MovementBehaviour* newBehaviour)
{
	if (m_pMovementBehavior != nullptr)
	{
		delete m_pMovementBehavior;
	}
	m_pMovementBehavior = newBehaviour;
}
void Enemy::SetFightingBehaviour(FightingBehaviour* newBehaviour)
{
	if (m_pFightingBehaviour != nullptr)
	{
		delete m_pFightingBehaviour;
	}
	m_pFightingBehaviour = newBehaviour;
}
void Enemy::SetDrawingBehaviour(DrawingBehaviour* newBehaviour)
{
	if (m_pDrawingBehaviour != nullptr)
	{
		delete m_pDrawingBehaviour;
	}
	m_pDrawingBehaviour = newBehaviour;
}

void Enemy::EquipGun(Gun* gun)
{
	if (m_pEquippedGun != nullptr)
	{
		delete m_pEquippedGun;
	}
	m_pEquippedGun = gun;
	m_pEquippedGun->SetHolder(this);
	m_pEquippedGun->UpdateGunPos(GetPosition());
}
Gun* Enemy::GetEquippedGun() const
{
	return m_pEquippedGun;
}