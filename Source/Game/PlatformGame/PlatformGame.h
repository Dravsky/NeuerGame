#pragma once
#include "Framework/Game.h"
#include "Framework/Event/EventManager.h"
#include "Renderer/Text.h"

class PlatformGame : public lola::Game, lola::IEventListener
{
public:
	enum class eState
	{
		Title,
		StartGame,
		StartLevel,
		Game,
		GameOver
	};

public:
	virtual bool Initialize() override;
	virtual void Shutdown() override;

	virtual void Update(float dt) override;
	virtual void Draw(lola::Renderer& renderer) override;

	void SetState(eState state) { m_state = state; }
	void OnPlayerWin(const lola::Event& event);

private:
	eState m_state = eState::Title;
	float m_spawnTimer = 0.0f;
	float m_spawnTime = 5.0f;
	bool victoryLap = false;

	float m_stateTimer = 0;
	float m_gameTimer = 0;
};
