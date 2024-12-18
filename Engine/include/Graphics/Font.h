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
#include <Detail/RenderSystem.h>

#include <LSD/Array.h>
#include <LSD/StringView.h>

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

#include <cstdint>

namespace esengine {

#ifdef ESENGINE_FONT_STORE_IN_ARRAY

class Font {
public:
	Font(lsd::StringView texturePath, glm::uvec2 charSize, std::uint32_t padding) noexcept;

	[[nodiscard]] const glm::ivec2& charSize() const noexcept {
		return m_charSize;
	}
	[[nodiscard]] SDL_Texture* texture() noexcept {
		return m_texture.texture();
	}

	[[nodiscard]] const SDL_FRect& at(int character) const noexcept;

private:
	Texture m_texture;

	lsd::Array <SDL_FRect, 256> m_rects;

	glm::uvec2 m_charSize;
	std::uint32_t m_padding;
};

#else

class Font {
public:
	Font(lsd::StringView texturePath, glm::uvec2 charSize, std::uint32_t padding) noexcept;

	[[nodiscard]] const glm::ivec2& charSize() const noexcept {
		return m_charSize;
	}
	[[nodiscard]] SDL_Texture* texture() noexcept {
		return m_texture.texture();
	}

	[[nodiscard]] SDL_FRect at(int c) const noexcept;

private:
	Texture m_texture;

	glm::ivec2 m_charSize;
	std::int32_t m_padding;
};

#endif

} // namespace esengine
