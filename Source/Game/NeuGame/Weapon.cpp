#include "Weapon.h"
#include "Renderer/Renderer.h"
#include "Framework/Framework.h"

namespace lola
{
	CLASS_DEFINITION(Weapon);

	bool Weapon::Initialize()
	{
		Actor::Initialize();

		auto collisionComponent = GetComponent<CollisionComponent>();
		if (collisionComponent)
		{
			auto renderComponent = GetComponent<RenderComponent>();
			if (renderComponent)
			{
				float scale = transform.scale;
				collisionComponent->m_radius = renderComponent->GetRadius() * scale;
			}
		}

		return true;
	}

	void Weapon::Update(float dt)
	{
		Actor::Update(dt);

		lola::vec2 forward = lola::vec2(0, -1).Rotate(transform.rotation);
		transform.position += forward * speed * lola::g_time.GetDeltaTime();
		transform.position.x = lola::Wrap(transform.position.x, (float)lola::g_renderer.GetWidth());
		transform.position.y = lola::Wrap(transform.position.y, (float)lola::g_renderer.GetHeight());
	}

	void Weapon::OnCollision(Actor* actor)
	{
		if (actor->tag != tag)
		{
			destroyed = true;
		}
	}

	void Weapon::Read(const json_t& value)
	{
		Actor::Read(value);

		READ_DATA(value, speed);
	}
}
