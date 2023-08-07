#include "Font.h"
#include <SDL2-2.28.0/include/SDL_ttf.h>
namespace lola
{
	Font::Font(const std::string& filename, int fontSize)
	{
		Load(filename, fontSize);
	}

	Font::~Font()
	{
		if (m_ttfFont != nullptr) {
			TTF_CloseFont(m_ttfFont);
		}
	}

	void Font::Load(const std::string& filename, int fontSize)
	{
		m_ttfFont = TTF_OpenFont(filename.c_str(), fontSize);
	}

	bool Font::Create(std::string filename, ...)
	{
		va_list args;
		va_start(args, filename);  // Initialize the va_list with the last known fixed argument

		// va_arg - accesses the next variadic function arguments
		int fontSize = va_arg(args, int);

		va_end(args);  // Clean up the va_list

		Load(filename, fontSize);

		// Check if m_ttfFont is not nullptr
		return m_ttfFont != nullptr;
	}

}
