#pragma once
#include "Framework/Actor.h"

class Powerup : public lola::Actor
{
public:
	Powerup(float speed, float turnRate, const lola::Transform& transform, std::shared_ptr<lola::Model> model) :
		Actor{ transform, model },
		m_speed{ speed },
		m_turnRate{ turnRate }
	{}

	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

private:
	float m_speed = 0;
	float m_health = 0;
	float m_turnRate = 0;
};
