#include "SpaceGame.h"
#include "Player.h"
#include "Enemy.h"
#include "Powerup.h"

#include "Framework/Scene.h"
#include "Framework/Emitter.h"
#include "Framework/Resource/ResourceManager.h"

#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"

#include "Renderer/Renderer.h"
#include "Renderer/Text.h"
#include "Renderer/ModelManager.h"
#include "Renderer/Particle.h"
#include "Renderer/ParticleSystem.h"


bool SpaceGame::Initialize()
{
	// create font / text objects
	m_font = lola::g_resources.Get<lola::Font>("arcadeclassic.ttf", 24);

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

	m_scene = std::make_unique<lola::Scene>();

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
	{
		std::unique_ptr<Player> player = std::make_unique<Player>(200.0f, lola::Pi, lola::Transform{ { 400, 300 }, 0, 6 }, lola::g_manager.Get("ship.txt"));
		player->m_tag = "Player";
		player->m_game = this;
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
				std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(lola::randomf(75.0f, 140.0f), lola::Pi, lola::Transform{ { lola::randomf((float)lola::g_renderer.GetHeight()), lola::randomf((float)lola::g_renderer.GetWidth()) }, lola::randomf(lola::TwoPi), 5.0f }, lola::g_manager.Get("enemy.txt"));
				enemy->m_tag = "Enemy";
				enemy->m_game = this;
				m_scene->Add(move(enemy));
			} 
			
			if (m_rounds % 3 == 0) {
				std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(lola::randomf(75.0f, 140.0f), lola::Pi, lola::Transform{ { lola::randomf((float)lola::g_renderer.GetHeight()), lola::randomf((float)lola::g_renderer.GetWidth()) }, lola::randomf(lola::TwoPi), 5.0f }, lola::g_manager.Get("berserk.txt"));
				enemy->m_tag = "Enemy";
				enemy->berserk = true;
				enemy->m_game = this;
				m_scene->Add(move(enemy));
			}

			m_rounds++;

			if (m_rounds % 2 == 0) {

				std::unique_ptr<Powerup> powerup = std::make_unique<Powerup>(lola::randomf(75.0f, 140.0f), lola::Pi, lola::Transform{ { lola::randomf((float)lola::g_renderer.GetHeight()), lola::randomf((float)lola::g_renderer.GetWidth()) }, lola::randomf(lola::TwoPi), 8.0f }, lola::g_manager.Get("f.txt"));
				powerup->m_tag = "Fireup";
				powerup->m_game = this;
				m_scene->Add(move(powerup));
			}
			else {

				std::unique_ptr<Powerup> powerup = std::make_unique<Powerup>(lola::randomf(75.0f, 140.0f), lola::Pi, lola::Transform{ { lola::randomf((float)lola::g_renderer.GetHeight()), lola::randomf((float)lola::g_renderer.GetWidth()) }, lola::randomf(lola::TwoPi), 3.0f }, lola::g_manager.Get("s.txt"));
				powerup->m_tag = "Speedup";
				powerup->m_game = this;
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

	m_scoreText->Draw(renderer, 40, 40);
	m_livesText->Draw(renderer, 40, 80);
	m_multiplierText->Draw(renderer, 40, 120);
	m_roundText->Draw(renderer, 40, 160);


	m_scene->Draw(renderer);
}