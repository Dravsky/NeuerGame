#include "Player.h"
#include "Weapon.h"
#include "SpaceGame.h"
#include "Framework/Scene.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include "Framework/Emitter.h"
#include "Renderer/ModelManager.h"

#include <iostream>


void Player::Update(float dt)
{
	Actor::Update(dt);

	cooldown -= dt;

	// Movement
	float rotate = 0;
	if (lola::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
	if (lola::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
	m_transform.rotation += rotate * m_turnRate * lola::g_time.GetDeltaTime();

	float thrust = 0;
	if (lola::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) thrust = 1;

	lola::vec2 forward = lola::vec2(0, -1).Rotate(m_transform.rotation);
	m_transform.position += forward * m_speed * thrust * lola::g_time.GetDeltaTime();
	m_transform.position.x = lola::Wrap(m_transform.position.x, (float)lola::g_renderer.GetWidth());
	m_transform.position.y = lola::Wrap(m_transform.position.y, (float)lola::g_renderer.GetHeight());

	// Fire Weapon
	if (lola::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !lola::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE))
	{
		// Create weapon
		lola::Transform transform{ m_transform.position, m_transform.rotation, 1 };
		std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>( 400.0f, transform, m_model );
		weapon->m_tag = "Player";
		m_scene->Add(std::move(weapon));
	}

	if (fireup)
	{
		// Create weapon
		lola::Transform transform{ m_transform.position, lola::randomf(1, 360), 1 };
		std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(400.0f, transform, m_model);
		weapon->m_lifespan = 0.2f;
		weapon->m_tag = "Player";
		m_scene->Add(std::move(weapon));
	}
}

void Player::OnCollision(Actor* actor)
{
	if (actor->m_tag == "Enemy")
	{
		if (cooldown <= 0)
		{
			m_health--;
			cooldown = hittimer;
		}

		m_model = lola::g_manager.Get("shipred.txt");

		if (m_health <= 0) 
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

			m_game->SetLives(m_game->GetLives() - 1);
			m_game->SetMultiplier(1);
			dynamic_cast<SpaceGame*>(m_game)->SetState(SpaceGame::eState::PlayerDead);
			m_destroyed = true;
		}
	}

	if (actor->m_tag == "Speedup")
	{
		m_speed += 100;
	}

	if (actor->m_tag == "Fireup")
	{
		fireup = true;
	}


}
