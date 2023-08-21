#include "Player.h"
#include "Weapon.h"
#include "SpaceGame.h"
#include "Framework/Scene.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include "Framework/Emitter.h"
#include "Framework/Components/SpriteComponent.h"
#include "Renderer/Texture.h"
#include "Framework/Resource/ResourceManager.h"
#include "Framework/Components/PhysicsComponent.h"

#include <iostream>
#include <Framework/Components/CircleCollisionComponent.h>


bool Player::Initialize()
{
	Actor::Initialize();

	// Cache off (saving off) a pointer to use later.
	 m_physicsComponent = GetComponent<lola::PhysicsComponent>();
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

void Player::Update(float dt)
{
	Actor::Update(dt);

	cooldown -= dt;

	// Movement
	float rotate = 0;
	if (lola::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
	if (lola::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
	transform.rotation += rotate * m_turnRate * lola::g_time.GetDeltaTime();

	float thrust = 0;
	if (lola::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) thrust = 1;
	lola::vec2 forward = lola::vec2(0, -1).Rotate(transform.rotation);

	m_physicsComponent->ApplyForce(forward * m_speed * thrust);

	transform.position.x = lola::Wrap(transform.position.x, (float)lola::g_renderer.GetWidth());
	transform.position.y = lola::Wrap(transform.position.y, (float)lola::g_renderer.GetHeight());

	// Fire Weapon
	if (lola::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !lola::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE))
	{
		auto weapon = INSTANTIATE(Weapon, "Rocket");
		weapon->transform = { transform.position, transform.rotation + lola::DegreesToRadians(10.0f), 1 };
		weapon->Initialize();
		m_scene->Add(std::move(weapon));
	}

	if (fireup)
	{
		auto weapon = INSTANTIATE(Weapon, "Rocket");
		weapon->transform = { transform.position, transform.rotation + lola::randomf(1, 360), 1 };
		weapon->Initialize();
		weapon->lifespan = 0.35f;
		m_scene->Add(std::move(weapon));
	}
}

void Player::OnCollision(Actor* actor)
{
	if (actor->tag == "Enemy")
	{
		if (cooldown <= 0)
		{
			m_health--;
			cooldown = hittimer;
		}

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
			auto emitter = std::make_unique<lola::Emitter>(this->transform, data);
			emitter->lifespan = 1.0f;
			m_scene->Add(std::move(emitter));

			m_game->SetLives(m_game->GetLives() - 1);
			m_game->SetMultiplier(1);
			dynamic_cast<SpaceGame*>(m_game)->SetState(SpaceGame::eState::PlayerDead);
			destroyed = true;
		}
	}

	if (actor->tag == "Speedup")
	{
		m_speed += 100;
	}

	if (actor->tag == "Fireup")
	{
		fireup = true;
	}


}
