#include "PlatformGame/Enemy.h"
#include "PlatformGame/Player.h"

#include "Input/InputSystem.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"

namespace lola
{
	CLASS_DEFINITION(Enemy);

	bool Enemy::Initialize()
	{
		Actor::Initialize();

		m_physicsComponent = GetComponent<lola::PhysicsComponent>();

		return true;
	}

	void Enemy::Update(float dt)
	{
		Actor::Update(dt);

		// Get player reference
		Player* player = m_scene->GetActor<Player>();
		if (player)
		{
			// Calculate the direction to the player
			lola::vec2 direction = player->transform.position - this->transform.position;
			direction.Normalize(); // Normalize the vector

			// Set the bat's velocity towards the player
			m_physicsComponent->SetVelocity(direction * speed);

			// Rotate bat to face the player
			transform.rotation = direction.Angle() + lola::HalfPi;
		}
	}

	void Enemy::OnCollisionEnter(Actor* actor)
	{
		if (actor->tag == "Player")
		{

		}

	}

	void Enemy::OnCollisionExit(Actor* actor)
	{

	}

	void Enemy::Read(const json_t& value)
	{
		Actor::Read(value);

		READ_NAME_DATA(value, "speed", speed);
		READ_NAME_DATA(value, "jump", jump);
	}
}
