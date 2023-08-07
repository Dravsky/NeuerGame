#include "Enemy.h"
#include "Player.h"
#include "Weapon.h"
#include "SpaceGame.h"
#include "Framework/Scene.h"
#include "Renderer/Renderer.h"
#include "Audio/AudioSystem.h"
#include "Framework/Emitter.h"

void Enemy::Update(float dt) 
{
	Actor::Update(dt);

	Player* player = m_scene->GetActor<Player>();
	if (player) 
	{
		lola::Vector2 direction = player->m_transform.position - this->m_transform.position;
		m_transform.rotation = direction.Angle() + lola::HalfPi;
	}

	lola::vec2 forward = lola::vec2(0, -1).Rotate(m_transform.rotation);
	m_transform.position += forward * m_speed * lola::g_time.GetDeltaTime();
	m_transform.position.x = lola::Wrap(m_transform.position.x, (float)lola::g_renderer.GetWidth());
	m_transform.position.y = lola::Wrap(m_transform.position.y, (float)lola::g_renderer.GetHeight());

	if (!berserk) {
		if (m_fireTimer <= 0)
		{
			// Create weapon
			lola::g_audioSystem.PlayOneShot("pew");
			lola::Transform transform{ m_transform.position, m_transform.rotation, 1 };
			std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(400.0f, transform, m_model);
			weapon->m_tag = "Enemy";
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
	lola::Transform transform{ m_transform.position, lola::randomf(1, 360), 1 };
	std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(400.0f, transform, m_model);
	weapon->m_lifespan = 0.2f;
	weapon->m_tag = "Enemy";
	m_scene->Add(std::move(weapon));
	}
	else
	{
		m_fireTimer -= dt;
	}
}

void Enemy::OnCollision(Actor* actor)
{
	if (actor->m_tag == "Player")
	{
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
		auto emitter = std::make_unique<lola::Emitter>(this->m_transform, data);
		emitter->m_lifespan = 1.0f;
		m_scene->Add(std::move(emitter));

		m_game->AddPoints(100 * m_game->GetMultiplier());
		m_destroyed = true;
	}
}
