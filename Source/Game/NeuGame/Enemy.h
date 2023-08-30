#pragma once
#include "Framework/Actor.h"
#include "Framework/Components/PhysicsComponent.h"

namespace lola {

	class Enemy : public Actor
	{
	public:
		CLASS_DECLARATION(Enemy);

		bool Initialize() override;

		void Update(float dt) override;
		void OnCollisionEnter(Actor* actor) override;

		bool berserk = false;

	private:
		float m_speed = 0;
		float m_turnRate = 0;

		float m_fireRate = 2;
		float m_fireTimer = 2;

		PhysicsComponent* m_physicsComponent = nullptr;
	};

}