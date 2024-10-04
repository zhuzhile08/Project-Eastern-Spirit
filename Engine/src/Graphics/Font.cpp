
#include <Graphics/Font.h>

namespace esengine {

Font::Font(glm::uvec2 charSize, std::uint32_t padding, Texture texturePath) : m_charSize(charSize), m_padding(padding), m_texturePath(texturePath) { 

	for (int i = 0; i <= 255; i++) {
		SDL_Rect rect;

		rect.w = charSize.x;
		rect.h = charSize.y;

		rect.x = (i % (texturePath.dimension().x / (charSize.x + padding))) * (charSize.x + padding);
		rect.y = (i * (charSize.y + padding) / texturePath.dimension().x) * (charSize.y + padding);

		m_rects[i] = rect;
	}
}

} //namespace esengine
