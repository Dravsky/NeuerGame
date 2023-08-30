#include "Weapon.h"
#include "Renderer/Renderer.h"
#include "Framework/Framework.h"

namespace lola
{
	CLASS_DEFINITION(Weapon);

	bool Weapon::Initialize()
	{
		Actor::Initialize();

		m_physicsComponent = GetComponent<PhysicsComponent>();

		auto collisionComponent = GetComponent<CollisionComponent>();
		if (collisionComponent)
		{
			
		}

		return true;
	}

	void Weapon::Update(float dt)
	{
		Actor::Update(dt);

		lola::vec2 forward = lola::vec2(0, -1).Rotate(transform.rotation);
		m_physicsComponent->SetVelocity(forward * speed);

		transform.position.x = lola::Wrap(transform.position.x, (float)lola::g_renderer.GetWidth());
		transform.position.y = lola::Wrap(transform.position.y, (float)lola::g_renderer.GetHeight());
	}

	void Weapon::OnCollisionEnter(Actor* actor)
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
