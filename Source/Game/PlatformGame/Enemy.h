#pragma once
#include "Framework/Actor.h"
#include <Framework/Components/PhysicsComponent.h>

namespace lola {

	class Enemy : public lola::Actor
	{
	public:
		CLASS_DECLARATION(Enemy);

		bool Initialize() override;
		void Update(float dt) override;

		void OnCollisionEnter(Actor* actor) override;
		void OnCollisionExit(Actor* actor) override;

	private:
		float speed = 0;
		float jump = 0;

		PhysicsComponent* m_physicsComponent = nullptr;
	};

}