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
	m_font = GET_RESOURCE(lola::Font, "SpaceGame/Fonts/ArcadeClassic.ttf", 24);

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
	lola::g_audioSystem.AddAudio("pew", "SpaceGame/Audio/pew.wav");

	//load sprites
	lola::res_t<lola::Texture> texture = GET_RESOURCE(lola::Texture, "SpaceGame/Textures/ship.png", lola::g_renderer);
	lola::res_t<lola::Texture> texture2 = GET_RESOURCE(lola::Texture, "SpaceGame/Textures/rocket.png", lola::g_renderer);

	// create scene
	m_scene = std::make_unique<lola::Scene>();
	m_scene->Load("SpaceGame/Scenes/SpaceScene.json");
	m_scene->Initialize();
	
	// add events
	EVENT_SUBSCRIBE("OnAddPoints", SpaceGame::OnAddPoints);
	EVENT_SUBSCRIBE("OnPlayerDead", SpaceGame::OnPlayerDead);

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
		auto player = INSTANTIATE(Player, "Player");
		player->transform = lola::Transform{ { 400, 300 }, 0, 1 };
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
				auto enemy = INSTANTIATE(Enemy, "Enemy");
				enemy->transform = lola::Transform{ { lola::random(800), lola::random(600) }, lola::randomf(lola::TwoPi), 1 };
				enemy->Initialize();
				m_scene->Add(move(enemy));
			} 
			
			if (m_rounds % 3 == 0) {
				auto enemy = INSTANTIATE(Enemy, "Enemy");
				enemy->transform = lola::Transform{ { lola::random(800), lola::random(600) }, lola::randomf(lola::TwoPi), 1 };
				enemy->berserk = true;

				// Create components
				auto renderComponent = CREATE_CLASS(SpriteComponent);
				renderComponent->m_texture = GET_RESOURCE(lola::Texture, "SpaceGame/Textures/enemy2.png", lola::g_renderer);
				enemy->AddComponent(std::move(renderComponent));

				enemy->Initialize();
				m_scene->Add(move(enemy));
			}

			m_rounds++;

			if (m_rounds % 3 == 0) {
				auto powerup = INSTANTIATE(Powerup, "Powerup");
				powerup->transform = lola::Transform{ { lola::random(800), lola::random(600) }, lola::randomf(lola::TwoPi), 1 };
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
		if (m_lives <= 0) m_state = eState::GameOver;
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

void SpaceGame::OnAddPoints(const lola::Event& event)
{
	m_score += std::get<int>(event.data);
}

void SpaceGame::OnPlayerDead(const lola::Event& event) 
{
	m_lives--;

	if (m_lives <= 0)
	{
		m_state = eState::GameOver;
	}
	else 
	{
		m_state = eState::StartLevel;
	}
}