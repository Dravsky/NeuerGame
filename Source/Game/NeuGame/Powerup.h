#pragma once
#include "Framework/Actor.h"

namespace lola {
	class Powerup : public lola::Actor
	{
	public:
		CLASS_DECLARATION(Powerup);

		bool Initialize() override;

		void Update(float dt) override;
		void OnCollisionEnter(Actor* actor) override;

	private:
		float m_speed = 0;
		float m_health = 0;
		float m_turnRate = 0;
	};
}
