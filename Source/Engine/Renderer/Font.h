#pragma once
#include "Framework/Resource/Resource.h"
#include <string>
#include <cstdarg>
struct _TTF_Font;

namespace lola
{
	class Font : public Resource
	{
	public:
		Font() = default;
		Font(const std::string& filename, int fontSize);
		~Font();
		void Load(const std::string& filename, int fontSize);

		virtual bool Create(std::string filename, ...) override;

		friend class Text;

	private:
		_TTF_Font* m_ttfFont = nullptr;
	};
}
