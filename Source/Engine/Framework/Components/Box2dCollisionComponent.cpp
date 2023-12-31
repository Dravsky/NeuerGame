#include "Box2dCollisionComponent.h"
#include "Box2dPhysicsComponent.h"
#include "SpriteComponent.h"
#include "Framework/Actor.h"

namespace lola
{
	CLASS_DEFINITION(Box2dCollisionComponent);

		bool Box2dCollisionComponent::Initialize()
	{
		auto component = m_owner->GetComponent<Box2dPhysicsComponent>();
		if (component)
		{
			if (data.size.x == 0 && data.size.y == 0) 
			{
				auto spriteComponent = m_owner->GetComponent<SpriteComponent>();
				if (spriteComponent) 
				{
					data.size = vec2{ spriteComponent->source.w, spriteComponent->source.h };
				}
			}

			data.size = data.size * scaleOffset * m_owner->transform.scale;

			if (component->m_body->GetType() == b2_staticBody)
			{
				PhysicsSystem::Instance().SetCollisionBoxStatic(component->m_body, data, m_owner);
			}
			else
			{
				PhysicsSystem::Instance().SetCollisionBox(component->m_body, data, m_owner);
			}
		}

		return true;
	}

	void Box2dCollisionComponent::Update(float dt)
	{
	}

	void Box2dCollisionComponent::Read(const json_t& value)
	{
		READ_NAME_DATA(value, "size", data.size);
		READ_NAME_DATA(value, "density", data.density);
		READ_NAME_DATA(value, "friction", data.friction);
		READ_NAME_DATA(value, "restitution", data.restitution);
		READ_NAME_DATA(value, "isTrigger", data.isTrigger);

		READ_DATA(value, scaleOffset);
	}
}