#include "Powerup.h"

void Powerup::Update(float dt)
{
	Actor::Update(dt);

	lola::vec2 forward = lola::vec2(0, -1).Rotate(m_transform.rotation);
	m_transform.position += forward * m_speed * lola::g_time.GetDeltaTime();
	m_transform.position.x = lola::Wrap(m_transform.position.x, (float)lola::g_renderer.GetWidth());
	m_transform.position.y = lola::Wrap(m_transform.position.y, (float)lola::g_renderer.GetHeight());
}

void Powerup::OnCollision(Actor* actor)
{
	if (actor->m_tag != m_tag)
	{
		m_destroyed = true;
	}
}
