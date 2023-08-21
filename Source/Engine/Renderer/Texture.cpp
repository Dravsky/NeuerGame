#include "SDL2-2.28.0/include/SDL_Image.h"
#include "Texture.h"
#include "Renderer.h"
#include "Core/Logger.h"

namespace lola
{
	Texture::~Texture()
	{
		if (m_texture) SDL_DestroyTexture(m_texture);
	}

	bool Texture::Create(std::string filename, ...)
	{
		va_list args;

		va_start(args, filename);

		Renderer& renderer = va_arg(args, Renderer);

		va_end(args);

		return Load(filename, renderer);
	}

	bool Texture::Load(const std::string& filename, Renderer& renderer)
	{
		SDL_Surface* surface = IMG_Load(filename.c_str());
		if (!surface)
		{
			WARNING_LOG("Surface not found.");			
			return false;
		}
		m_texture = SDL_CreateTextureFromSurface(renderer.m_renderer, surface);
		SDL_FreeSurface(surface);
		if (!m_texture)
		{
			WARNING_LOG("Texture not found.");
			return false;
		}
		return true;
	}

	vec2 Texture::GetSize()
	{
		ASSERT_LOG((m_texture != nullptr), "Texture " << m_texture << " not found.");
		SDL_Point point;
		
		int w, h;

		SDL_QueryTexture(m_texture, nullptr, nullptr, &w, &h);

		point.x = w;
		point.y = h;

		return vec2{ point.x, point.y };
	}

}