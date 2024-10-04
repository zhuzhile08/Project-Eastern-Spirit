/*************************
 * @file Font.h
 * @author Okeeee_Holz
 *
 * @brief Load font
 *
 * @date 2024-10-04
 *
 * @copyright Copyright (c) 2024
 *************************/

#pragma once

#include <Graphics/Texture.h>

#include <LSD/Array.h>

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

#include <cstddef>
#include <cstdio>

namespace esengine {

class Font {
public:
	Font(glm::uvec2 charSize, std::uint32_t padding, Texture texturePath) noexcept;

	[[nodiscard]] const SDL_Rect& at(int character) const noexcept{
		try {
			return m_rects.at(character);
		}
		catch (const std::out_of_range& e) {
			std::printf("esengine::Font::at(): %s\n", e.what());
			return m_rects.at(0);
		}
	}

private:
	Texture m_texturePath = Texture("data/reffspixelfont_spritesheet_0_to_1023.png");
	glm::uvec2 m_charSize = glm::uvec2(16, 16);
	std::uint32_t m_padding;
	lsd::Array <SDL_Rect, 256> m_rects;
};

} //namespace esengine
