#pragma once
#include "CollisionComponent.h"
#include "Physics/PhysicsSystem.h"
#include <functional>

namespace lola
{
	class Box2dCollisionComponent : public CollisionComponent
	{
	public:
		using collisionFunction = std::function<void(Actor*)>;

	public:
		CLASS_DECLARATION(Box2dCollisionComponent);

		virtual bool Initialize() override;
		virtual void Update(float dt) override;

	private:
		PhysicsSystem::CollisionData data;
		vec2 scaleOffset = vec2{ 1, 1 };
	};
}