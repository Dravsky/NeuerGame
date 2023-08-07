#pragma once
#include "Core/Core.h"
#include "Renderer/Model.h"
#include <memory>

namespace lola {
class Actor
{
	public:
		Actor() = default;
		Actor(const lola::Transform& transform, std::shared_ptr<Model> model) :
			m_transform{ transform },
			m_model{ model } 
		{}
		Actor(const lola::Transform& transform) :
			m_transform{ transform }
		{}

		virtual void Update(float dt);
		virtual void Draw(lola::Renderer& renderer);

		float GetRadius() { return (m_model) ? m_model->GetRadius() * m_transform.scale : 0; }		
		virtual void OnCollision(Actor* other) {}

		class Scene* m_scene = nullptr;
		friend class Scene;
		
		class Game* m_game = nullptr;

		lola::Transform m_transform;
		std::string m_tag;

		float m_lifespan = -1.0f;
		bool m_destroyed = false;

	protected:
		std::shared_ptr<Model> m_model;
	};
}