#pragma once
#include "Vector2.h"

namespace lola
{
	class Transform 
	{
	public: 
		vec2 position;
		float rotation = 0.0f;
		float scale = 1.0f;

	public:
		Transform() = default;
		Transform(const vec2& position, float rotation, float scale = 1.0f) :
			position{ position },
			rotation{ rotation },
			scale{ scale } {}
	};
}
