#pragma once
#include "Framework/Actor.h"

namespace lola 
{
	class Weapon : public Actor
	{
	public:
		CLASS_DECLARATION(Weapon);

		bool Initialize() override;
		void Update(float dt) override;

		void OnCollision(Actor* actor);

	private:
		float speed = 0;
	};
}