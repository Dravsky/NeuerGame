#include "Enemy.h"
#include "Player.h"
#include "Weapon.h"
#include "SpaceGame.h"
#include "Renderer/Renderer.h"
#include "Audio/AudioSystem.h"
#include "Framework/Framework.h"


bool Enemy::Initialize()
{
	Actor::Initialize();

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

	Player* player = m_scene->GetActor<Player>();
	if (player) 
	{
		lola::Vector2 direction = player->transform.position - this->transform.position;
		transform.rotation = direction.Angle() + lola::HalfPi;
	}

	lola::vec2 forward = lola::vec2(0, -1).Rotate(transform.rotation);
	transform.position += forward * m_speed * lola::g_time.GetDeltaTime();
	transform.position.x = lola::Wrap(transform.position.x, (float)lola::g_renderer.GetWidth());
	transform.position.y = lola::Wrap(transform.position.y, (float)lola::g_renderer.GetHeight());

	if (!berserk) {
		if (m_fireTimer <= 0)
		{
			// Create weapon
			//lola::g_audioSystem.PlayOneShot("pew");
			//std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(400.0f, transform);
			//weapon->tag = "Enemy";

			//std::unique_ptr<lola::SpriteComponent> component = std::make_unique<lola::SpriteComponent>();
			//component->m_texture = GET_RESOURCE(lola::Texture, "rocket.png", lola::g_renderer);
			//weapon->AddComponent(std::move(component));

			//m_scene->Add(std::move(weapon));
			//m_fireTimer = m_fireRate;
		}
		else
		{
			m_fireTimer -= dt;
		}
	}
	if (berserk && m_fireTimer <= 0) {
	// Create weapon
	//lola::Transform transform{ transform.position, lola::randomf(1, 360), 1 };
	//std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(400.0f, transform);
	//weapon->lifespan = 0.2f;
	//weapon->tag = "Enemy";

	//std::unique_ptr<lola::SpriteComponent> component = std::make_unique<lola::SpriteComponent>();
	//component->m_texture = GET_RESOURCE(lola::Texture, "rocket.png", lola::g_renderer);
	//weapon->AddComponent(std::move(component));

	//m_scene->Add(std::move(weapon));
	}
	else
	{
		m_fireTimer -= dt;
	}
}

void Enemy::OnCollision(Actor* actor)
{
	if (actor->tag == "Player")
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

		//
		auto emitter = std::make_unique<lola::Emitter>(this->transform, data);
		emitter->lifespan = 1.0f;
		m_scene->Add(std::move(emitter));

		m_game->AddPoints(100 * m_game->GetMultiplier());
		destroyed = true;
	}
}
