#include "Powerup.h"
#include "Renderer/Renderer.h"
#include "Framework/Framework.h"

bool Powerup::Initialize()
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

void Powerup::Update(float dt)
{
	Actor::Update(dt);

	lola::vec2 forward = lola::vec2(0, -1).Rotate(transform.rotation);
	transform.position += forward * m_speed * lola::g_time.GetDeltaTime();
	transform.position.x = lola::Wrap(transform.position.x, (float)lola::g_renderer.GetWidth());
	transform.position.y = lola::Wrap(transform.position.y, (float)lola::g_renderer.GetHeight());
}

void Powerup::OnCollision(Actor* actor)
{
	if (actor->tag != tag)
	{
		destroyed = true;
	}
}
