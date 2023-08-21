#include "SpaceGame.h"
#include "Player.h"
#include "Enemy.h"
#include "Powerup.h"

#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"

#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Core/Core.h"


bool SpaceGame::Initialize()
{
	// create font / text objects
	m_font = GET_RESOURCE(lola::Font, "arcadeclassic.ttf", 24);

	m_scoreText = std::make_unique<lola::Text>(m_font);
	m_scoreText->Create(lola::g_renderer, "0", lola::Color{ 1, 1, 1, 1 });

	m_titleText = std::make_unique<lola::Text>(m_font);
	m_titleText->Create(lola::g_renderer, "SPACER", lola::Color{ 1, 1, 1, 1 });

	m_livesText = std::make_unique<lola::Text>(m_font);
	m_livesText->Create(lola::g_renderer, "3", lola::Color{ 1, 1, 1, 1 });

	m_deadText = std::make_unique<lola::Text>(m_font);
	m_deadText->Create(lola::g_renderer, "GAMEOVER", lola::Color{ 1, 1, 1, 1 });

	m_multiplierText = std::make_unique<lola::Text>(m_font);
	m_multiplierText->Create(lola::g_renderer, "1", lola::Color{ 1, 1, 1, 1 });

	m_roundText = std::make_unique<lola::Text>(m_font);
	m_roundText->Create(lola::g_renderer, "1", lola::Color{ 1, 1, 1, 1 });

	// load auduio
	lola::g_audioSystem.AddAudio("pew", "pew.wav");

	//load sprites
	lola::res_t<lola::Texture> texture = GET_RESOURCE(lola::Texture, "ship.png", lola::g_renderer);
	lola::res_t<lola::Texture> texture2 = GET_RESOURCE(lola::Texture, "rocket.png", lola::g_renderer);

	// create scene
	m_scene = std::make_unique<lola::Scene>();
	m_scene->Load("scene.json");
	m_scene->Initialize();

	return true;
}

void SpaceGame::Shutdown()
{

}

void SpaceGame::Update(float dt)
{
	switch (m_state)
	{
	case SpaceGame::eState::Title:
		if (lola::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE)) 
		{
			m_state = eState::StartGame;
		}
		break;
	case SpaceGame::eState::StartGame:
		m_score = 0;
		m_lives = 3;
		m_multiplier = 1;
		m_rounds = 1;
		m_state = eState::StartLevel;
		break;

	case SpaceGame::eState::StartLevel: 
		m_rounds = 1;
		m_scene->RemoveAll();
		m_scene->GetActorByName("Title")->active = false;
	{
		// Create player
		std::unique_ptr<Player> player = std::make_unique<Player>(200.0f, lola::Pi, lola::Transform{ { 400, 300 }, 0, 1.0f });
		player->tag = "Player";
		player->m_game = this;

		// Create components
		auto renderComponent = CREATE_CLASS(SpriteComponent);
		renderComponent->m_texture = GET_RESOURCE(lola::Texture, "ship.png", lola::g_renderer);
		player->AddComponent(std::move(renderComponent));

		auto physicsComponent = CREATE_CLASS(EnginePhysicsComponent)
		physicsComponent->m_damping = 0.9999999702f;
		player->AddComponent(std::move(physicsComponent));
		
		auto collisionComponent = CREATE_CLASS(CircleCollisionComponent);
		collisionComponent->m_radius = 30.0f;
		player->AddComponent(std::move(collisionComponent));

		player->Initialize();
		m_scene->Add(move(player));

	}
		m_state = eState::Game;
		break;
	case SpaceGame::eState::Game:
		m_spawnTimer += dt;
		if (m_spawnTimer >= m_spawnTime) 
		{
			m_spawnTimer = 0.0f;
			
			for (size_t i = 0; i < m_rounds; i++)
			{
				std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(lola::randomf(75.0f, 140.0f), lola::Pi, lola::Transform{ { lola::randomf((float)lola::g_renderer.GetHeight()), lola::randomf((float)lola::g_renderer.GetWidth()) }, lola::randomf(lola::TwoPi), 1.0f });
				enemy->tag = "Enemy";
				enemy->m_game = this;
				// Create components
				auto renderComponent = CREATE_CLASS(SpriteComponent);
				renderComponent->m_texture = GET_RESOURCE(lola::Texture, "enemy.png", lola::g_renderer);
				enemy->AddComponent(std::move(renderComponent));

				auto collisionComponent = CREATE_CLASS(CircleCollisionComponent);
				collisionComponent->m_radius = 30.0f;
				enemy->AddComponent(std::move(collisionComponent));

				enemy->Initialize();
				m_scene->Add(move(enemy));
			} 
			
			if (m_rounds % 3 == 0) {
				std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(lola::randomf(75.0f, 140.0f), lola::Pi, lola::Transform{ { lola::randomf((float)lola::g_renderer.GetHeight()), lola::randomf((float)lola::g_renderer.GetWidth()) }, lola::randomf(lola::TwoPi), 1.0f });
				enemy->tag = "Enemy";
				enemy->berserk = true;
				enemy->m_game = this;

				// Create components
				auto renderComponent = CREATE_CLASS(SpriteComponent);
				renderComponent->m_texture = GET_RESOURCE(lola::Texture, "enemy2.png", lola::g_renderer);
				enemy->AddComponent(std::move(renderComponent));

				auto collisionComponent = CREATE_CLASS(CircleCollisionComponent);
				collisionComponent->m_radius = 30.0f;
				enemy->AddComponent(std::move(collisionComponent));

				enemy->Initialize();
				m_scene->Add(move(enemy));
			}

			m_rounds++;

			if (m_rounds % 2 == 0) {

				std::unique_ptr<Powerup> powerup = std::make_unique<Powerup>(lola::randomf(75.0f, 140.0f), lola::Pi, lola::Transform{ { lola::randomf((float)lola::g_renderer.GetHeight()), lola::randomf((float)lola::g_renderer.GetWidth()) }, lola::randomf(lola::TwoPi), 1.0f });
				powerup->tag = "Fireup";
				powerup->m_game = this;

				auto renderComponent = CREATE_CLASS(SpriteComponent);
				renderComponent->m_texture = GET_RESOURCE(lola::Texture, "f.png", lola::g_renderer);
				powerup->AddComponent(std::move(renderComponent));

				auto collisionComponent = CREATE_CLASS(CircleCollisionComponent);
				collisionComponent->m_radius = 30.0f;
				powerup->AddComponent(std::move(collisionComponent));

				powerup->Initialize();
				m_scene->Add(move(powerup));
			}
			else {

				std::unique_ptr<Powerup> powerup = std::make_unique<Powerup>(lola::randomf(75.0f, 140.0f), lola::Pi, lola::Transform{ { lola::randomf((float)lola::g_renderer.GetHeight()), lola::randomf((float)lola::g_renderer.GetWidth()) }, lola::randomf(lola::TwoPi), 1.0f });
				powerup->tag = "Speedup";
				powerup->m_game = this;

				auto renderComponent = CREATE_CLASS(SpriteComponent);
				renderComponent->m_texture = GET_RESOURCE(lola::Texture, "s.png", lola::g_renderer);
				powerup->AddComponent(std::move(renderComponent));

				auto collisionComponent = CREATE_CLASS(CircleCollisionComponent);
				collisionComponent->m_radius = 30.0f;
				powerup->AddComponent(std::move(collisionComponent));

				powerup->Initialize();
				m_scene->Add(move(powerup));
			}

		}

		if (lola::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !lola::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE))
		{
			lola::g_audioSystem.PlayOneShot("pew");
		}

		if (m_score == 400 && m_multiplier != 2 ) { m_multiplier++; }
		if (m_score == 1000 && m_multiplier != 3) { m_multiplier++; }

		break;
	case SpaceGame::eState::PlayerDead:
		if (m_lives == 0) m_state = eState::GameOver;
		else m_state = eState::StartLevel;

		break;
	case SpaceGame::eState::GameOver:
		break;
	default:
		break;
	}

	m_scoreText->Create(lola::g_renderer, "Score " + std::to_string(m_score), { 1, 1, 1, 1 });
	m_livesText->Create(lola::g_renderer, "Lives " + std::to_string(m_lives), {1, 1, 1, 1});
	m_multiplierText->Create(lola::g_renderer, "Multiplier x" + std::to_string(m_multiplier), { 1, 1, 1, 1 });
	m_roundText->Create(lola::g_renderer, "Round " + std::to_string( (m_rounds - 1) ), { 1, 1, 1, 1 });


	m_scene->Update(dt);
}

void SpaceGame::Draw(lola::Renderer& renderer)
{
	if (m_state == eState::Title) 
	{
		m_titleText->Draw(renderer, 350, 300);
	}

	if (m_state == eState::GameOver)
	{
		m_deadText->Draw(renderer, 350, 300);
	}

	m_scene->Draw(renderer);

	m_scoreText->Draw(renderer, 40, 40);
	m_livesText->Draw(renderer, 40, 80);
	m_multiplierText->Draw(renderer, 40, 120);
	m_roundText->Draw(renderer, 40, 160);
}