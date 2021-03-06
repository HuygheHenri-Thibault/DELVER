#pragma once
#include "Actor.h"
#include <vector>
#include "MovementBehaviour.h"
#include "FightingBehaviour.h"
#include "DrawingBehaviour.h"

class Gun;

class Enemy : public Actor
{
public:
	enum class Type
	{
		turret
	};

	enum class State
	{
		idle,
		deploying,
		fighting,
		undeploying,
		dead
	};

	struct BehaviourSet
	{
		MovementBehaviour* movementBehaviour = nullptr;
		FightingBehaviour* fightingBehaviour = nullptr;
		DrawingBehaviour* drawingBehaviour = nullptr;
	};

	State m_State;
	const int m_MaxHitPoints;
	int m_CurrentHitpoints;
	float m_DetectionRange;
	Actor* m_pTarget;
	bool m_CanDelete;
	bool m_DeathHandled;

	Enemy::Type m_EnemyType;

	Enemy(const ActorData& actorData, const BehaviourSet& behaviours, float detectionRange, int hitPoints, Gun* gun = nullptr);
	Enemy(const Enemy& other) = delete;
	Enemy& operator=(const Enemy& other) = delete;
	Enemy(Enemy&& other) = delete;
	Enemy& operator=(Enemy&& other) = delete;
	virtual ~Enemy();

	virtual void Update(float elapsedSec, const Level& level);
	virtual void Draw() const override;

	bool IsTargetInRange(const Actor& target) const;

	bool IsDead() const;
	void HandleDeath();

	void SetBehaviour(const Enemy::BehaviourSet& behaviour);
	void SetMovementBehaviour(MovementBehaviour* newBehaviour);
	void SetFightingBehaviour(FightingBehaviour* newBehaviour);
	void SetDrawingBehaviour(DrawingBehaviour* newBehaviour);

	void EquipGun(Gun* gun);
	Gun* GetEquippedGun() const;

protected:
	Gun* m_pEquippedGun;
	MovementBehaviour* m_pMovementBehavior;
	FightingBehaviour* m_pFightingBehaviour;
	DrawingBehaviour* m_pDrawingBehaviour;

private:
	void Initialize();
};
