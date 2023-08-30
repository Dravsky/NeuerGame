#pragma once
#include "Framework/Game.h"
#include "Framework/Event/EventManager.h"
#include "Renderer/Text.h"

class SpaceGame : public lola::Game, lola::IEventListener
{
public:
	enum class eState
	{
		Title,
		StartGame,
		StartLevel,
		Game,
		PlayerDeadStart,
		PlayerDead,
		GameOverStart,
		GameOver
	};

public:
		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float dt) override;
		virtual void Draw(lola::Renderer& renderer) override;

		void SetState(eState state) { m_state = state; }
		void OnAddPoints(const lola::Event& event);
		void OnPlayerDead(const lola::Event& event);

private:
	eState m_state = eState::Title;
	float m_spawnTimer = 0.0f;
	float m_spawnTime = 5.0f;

	std::shared_ptr<lola::Font> m_font;
	std::unique_ptr<lola::Text> m_scoreText;
	std::unique_ptr<lola::Text> m_livesText;
	std::unique_ptr<lola::Text> m_titleText;
	std::unique_ptr<lola::Text> m_deadText;
	std::unique_ptr<lola::Text> m_multiplierText;
	std::unique_ptr<lola::Text> m_healthText;
	std::unique_ptr<lola::Text> m_roundText;

};
