#pragma once
#include "Framework/Resource/Resource.h"
#include "Core/Core.h"
#include <vector>

namespace lola
{
	class Renderer;

	class Model : public Resource
	{
	public:
		Model() = default;
		Model(const std::vector<Vector2>& points) : m_points{ points } {}

		virtual bool Create(std::string filename, ...) override;
		bool Load(const std::string& filename);

		void Draw(Renderer& renderer, const Vector2& position, float rotation, float scale);
		void Draw(Renderer& renderer, const Transform& transform);

		float GetRadius();

	private:
		std::vector<Vector2> m_points;
		Color m_color;
		float m_radius = 0;
	};
}