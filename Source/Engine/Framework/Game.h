#pragma once
#include <memory>

namespace lola 
{
	class Game 
	{
	public:
		Game() = default;
		virtual ~Game() = default;

		virtual bool Initialize() = 0;
		virtual void Shutdown() = 0;

		virtual void Update(float dt) = 0;
		virtual void Draw(class Renderer& renderer) = 0;

		int GetScore() const { return m_score; }
		void AddPoints(int points) { m_score += points; }

		int GetLives() const { return m_lives; }
		void SetLives(int lives) { m_lives = lives; }

		int GetMultiplier() const { return m_multiplier; }
		void SetMultiplier(int multiplier) { m_multiplier = multiplier; }
		
	protected:
		std::unique_ptr<class Scene> m_scene;

		int m_score = 0;
		int m_lives = 0;
		int m_multiplier = 0;
		int m_rounds = 0;
	};


}