
#include <Graphics/Font.h>

#include <cstdio>

namespace esengine {

#ifdef ESENGINE_FONT_STORE_IN_ARRAY

Font::Font(lsd::StringView texturePath, glm::uvec2 charSize, std::uint32_t padding) noexcept : 
	m_texture(texturePath),
	m_charSize(charSize), 
	m_padding(padding) {
	for (int i = 0; i <= 255; i++) {
		m_rects[i] = SDL_Rect {
			((i * (m_charSize.x + m_padding)) % (m_texture.dimension().x)),
			(i * (m_charSize.y + m_padding) / m_texture.dimension().x) * (m_charSize.y + m_padding),
			m_charSize.x,
			m_charSize.y
		}
	}
}

const SDL_Rect& Font::at(int character) const noexcept {
	try {
		return m_rects.at(character);
	}
	catch (const std::out_of_range& e) {
		std::printf("esengine::Font::at(): %s\n", e.what());
		return m_rects.at(0);
	}
}

#else

Font::Font(lsd::StringView texturePath, glm::uvec2 charSize, std::uint32_t padding) noexcept : 
	m_texture(texturePath),
	m_charSize(charSize), 
	m_padding(padding) { }

SDL_Rect Font::at(int c) const noexcept {
	return SDL_Rect {
		((c * (m_charSize.x + m_padding)) % (m_texture.dimension().x)),
		(c * (m_charSize.y + m_padding) / m_texture.dimension().x) * (m_charSize.y + m_padding),
		m_charSize.x,
		m_charSize.y
	};
}

#endif

} //namespace esengine
