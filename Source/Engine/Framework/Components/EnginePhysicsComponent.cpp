#include "EnginePhysicsComponent.h"
#include "Framework/Actor.h"

namespace lola 
{
	CLASS_DEFINITION(EnginePhysicsComponent);

	void EnginePhysicsComponent::Update(float dt)
	{
		m_owner->transform.position += velocity * dt;
		velocity *= std::pow(1.0f - m_damping, dt);
	}

	void EnginePhysicsComponent::ApplyForce(const vec2& force)
	{
		velocity += force;
	}

	void EnginePhysicsComponent::ApplyTorque(float torque)
	{

	}

	void EnginePhysicsComponent::Read(const json_t& value) 
	{
		
	}
}