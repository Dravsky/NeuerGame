#pragma once
#include "Framework/Actor.h"
#include <Framework/Components/PhysicsComponent.h>
#include <Framework/Components/SpriteAnimComponent.h>

namespace lola {

	class Player : public lola::Actor
	{
	public:
		CLASS_DECLARATION(Player);

		bool Initialize() override;
		void Update(float dt) override;

		void OnCollisionEnter(Actor* actor) override;
		void OnCollisionExit(Actor* actor) override;

	private:
		float speed = 0;
		float jump = 0;
		int groundCount = 0;

		PhysicsComponent* m_physicsComponent = nullptr;
		SpriteAnimComponent* m_spriteAnimComponent = nullptr;
	};

}