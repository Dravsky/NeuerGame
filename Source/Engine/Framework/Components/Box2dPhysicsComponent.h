#pragma once
#include "PhysicsComponent.h"
#include "Physics/PhysicsSystem.h"

class b2Body;

namespace lola
{
	class Box2dPhysicsComponent : public PhysicsComponent
	{
	public:
		CLASS_DECLARATION(Box2dPhysicsComponent);

		bool Initialize() override;
		void OnDestroy() override;

		void Update(float dt) override;

		virtual void ApplyForce(const vec2& force) override;
		virtual void ApplyTorque(float torque) override;
		virtual void SetVelocity(const vec2& velocity) override;

		friend class Box2dCollisionComponent;

	private:
		PhysicsSystem::RigidBodyData data;
		b2Body* m_body = nullptr;
	};
}