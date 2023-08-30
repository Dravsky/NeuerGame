#include "NeuGame/Enemy.h"
#include "Player.h"
#include "Weapon.h"
#include "SpaceGame.h"
#include "Renderer/Renderer.h"
#include "Audio/AudioSystem.h"
#include "Framework/Framework.h"

namespace lola 
{
	CLASS_DEFINITION(Enemy);

	bool Enemy::Initialize()
	{
		Actor::Initialize();

		m_physicsComponent = GetComponent<PhysicsComponent>();
		auto collisionComponent = GetComponent<lola::CollisionComponent>();
		if (collisionComponent)
		{
			auto renderComponent = GetComponent<lola::RenderComponent>();
			if (renderComponent)
			{
				float scale = transform.scale;
				collisionComponent->m_radius = renderComponent->GetRadius() * scale;
			}
		}

		return true;
	}

	void Enemy::Update(float dt)
	{
		Actor::Update(dt);

		lola::vec2 forward = lola::vec2(0, -1).Rotate(transform.rotation);

		Player* player = m_scene->GetActor<Player>();
		if (player)
		{
			lola::Vector2 direction = player->transform.position - this->transform.position;
			transform.rotation = direction.Angle() + lola::HalfPi;

			float turnAngle = lola::vec2::SignedAngle(forward, direction.Normalized());
			m_physicsComponent->ApplyTorque(turnAngle);
		}

		m_physicsComponent->ApplyForce(forward * m_speed);

		transform.position.x = lola::Wrap(transform.position.x, (float)lola::g_renderer.GetWidth());
		transform.position.y = lola::Wrap(transform.position.y, (float)lola::g_renderer.GetHeight());

		if (!berserk) {
			if (m_fireTimer <= 0)
			{
				// Create weapon
				auto weapon = INSTANTIATE(Weapon, "Rocket");
				weapon->transform = { transform.position, transform.rotation + lola::DegreesToRadians(10.0f), 1 };
				weapon->tag = "Enemy";
				weapon->Initialize();
				m_scene->Add(std::move(weapon));
				m_fireTimer = m_fireRate;
			}
			else
			{
				m_fireTimer -= dt;
			}
		}
		if (berserk && m_fireTimer <= 0) {
			// Create weapon
			auto weapon = INSTANTIATE(Weapon, "Rocket");
			weapon->tag = "Enemy";
			weapon->transform = { transform.position, transform.rotation + lola::randomf(1, 360), 1 };
			weapon->Initialize();
			weapon->lifespan = 0.1f;
			m_scene->Add(std::move(weapon));
		}
		else
		{
			m_fireTimer -= dt;
		}
	}

	void Enemy::OnCollisionEnter(Actor* actor)
	{
		if (actor->tag == "Player")
		{
			lola::EventManager::Instance().DispatchEvent("AddPoints", 100);
			destroyed = true;

			lola::EmitterData data;
			data.burst = true;
			data.burstCount = 100;
			data.spawnRate = 200;
			data.angle = 0;
			data.angleRange = lola::Pi;
			data.lifetimeMin = 0.5f;
			data.lifetimeMin = 1.5f;
			data.speedMin = 50;
			data.speedMax = 250;
			data.damping = 0.5f;

			data.color = lola::Color{ 1, 1, 1, 1 };

			lola::Transform transform{ this->transform.position, 0, 1 };
			auto emitter = std::make_unique<lola::Emitter>(this->transform, data);
			emitter->lifespan = 1.0f;
			m_scene->Add(std::move(emitter));
		}
	}

	void Enemy::Read(const json_t& value)
	{
		Actor::Read(value);

		READ_NAME_DATA(value, "speed", m_speed);
		READ_NAME_DATA(value, "turnRate", m_turnRate);
		READ_NAME_DATA(value, "fireRate", m_fireRate);
		READ_NAME_DATA(value, "fireTimer", m_fireTimer);
	}
}