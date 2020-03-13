#pragma once
#include "Actor.h"
#include <vector>

class Gun;

class Player : public Actor
{
public: 
	enum class State
	{
		waiting,
		moving,
		dead
	};

	Player(const Point2f& pos, Gun* pGunEquiped = nullptr);
	~Player();

	void Update(float elapsedSec) override;
	void Update(float elapsedSec, const Point2f mousePos);
	void Draw() const override;

	void EquipGun(Gun* pGunToEquip);
	void SwapGun(bool swappingToNext);
	Gun* GetEquippedGun() const;

private:
	State m_State;
	std::vector<Gun*> m_pGuns;
	size_t m_IdxEquippedGun;
};

