#pragma once
#include "Vector2.h"
#include "Matrix22.h"
#include "Matrix33.h"
#include "Json.h"

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
			scale{ scale } 
		{}

		mat3 getMatrix() const
		{
			mat3 ms = mat3::CreateScale(scale);
			mat3 mr = mat3::CreateRotation(rotation);
			mat3 mt = mat3::CreateTranslation(position);
			mat3 mx = mt * ms * mr;

				return mx;
		}

		void Read(const json_t& value);
	};
}
