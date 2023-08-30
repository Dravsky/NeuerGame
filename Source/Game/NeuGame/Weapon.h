#pragma once
#include "Framework/Actor.h"
#include <Framework/Components/PhysicsComponent.h>

namespace lola 
{
	class Weapon : public Actor
	{
	public:
		CLASS_DECLARATION(Weapon);

		bool Initialize() override;
		void Update(float dt) override;

		void OnCollisionEnter(Actor* actor);

	private:
		float speed = 0;
		PhysicsComponent * m_physicsComponent = nullptr;
	};
}