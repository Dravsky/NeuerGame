#pragma once
#include "Framework/Actor.h"

class Powerup : public lola::Actor
{
public:
	Powerup(float speed, float turnRate, const lola::Transform& transform) :
		Actor{ transform },
		m_speed{ speed },
		m_turnRate{ turnRate }
	{}

	bool Initialize() override;

	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

private:
	float m_speed = 0;
	float m_health = 0;
	float m_turnRate = 0;
};
