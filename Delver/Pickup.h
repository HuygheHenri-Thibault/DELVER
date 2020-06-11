#pragma once
#include "Item.h"

class Pickup final : public Item
{
public:
	enum class PickupType
	{
		bounce,
		warp
	};

	Pickup(PickupType type, float effectDuration, const Point2f& pos, const Vector2f& velocity = Vector2f{ 0,0 });
	Pickup(const std::string& stringData);
	Pickup(const Pickup& other);
	Pickup& operator=(const Pickup& other) = delete;
	Pickup(Pickup&& other) = delete;
	Pickup& operator=(Pickup&& other) = delete;
	~Pickup();

	virtual void Update(float elapsedSec, Player& player) override;
	virtual void Draw() const override;
	virtual void OnPickup(Player& player) override;

	void StartEffect();
	void StopEffect();
	// StartEffect();
	// StopEffect();

private:
	PickupType m_Type;
	float m_EffectDuration;
	float m_EffectTimer;
	Player* m_pAffectedPlayer;
	Texture* m_pTexture;
	bool m_EffectActive;
};

