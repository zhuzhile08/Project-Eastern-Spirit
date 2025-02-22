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

#include <LSD/Vector.h>
#include <LSD/StringView.h>

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

#include <cstdint>

namespace esengine {

class Font {
public:
	Font(lsd::StringView fontPath);

	[[nodiscard]] const glm::ivec2& cellSize() const noexcept {
		return m_cellSize;
	}
	[[nodiscard]] std::size_t baseLine() const noexcept {
		return m_baseLine;
	}
	[[nodiscard]] std::size_t capLine() const noexcept {
		return m_capLine;
	}

	[[nodiscard]] Texture& texture() noexcept {
		return m_texture;
	}
	[[nodiscard]] const Texture& texture() const noexcept {
		return m_texture;
	}

	[[nodiscard]] const SDL_FRect& at(int c) const noexcept;

private:
	Texture m_texture;

	glm::ivec2 m_cellSize;
	std::size_t m_baseLine;
	std::size_t m_capLine;

	lsd::Vector<SDL_FRect> m_chars;
};

} // namespace esengine
