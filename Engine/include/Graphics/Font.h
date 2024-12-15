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
#include <Graphics/RenderSystem.h>

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

	[[nodiscard]] RenderSystem::CallData drawCall(const glm::mat4& transform, const Textbox& textBox) const;

private:
	Texture m_texture;

	lsd::Array <SDL_Rect, 256> m_rects;

	glm::uvec2 m_charSize;
	std::uint32_t m_padding;

	[[nodiscard]] const SDL_Rect& at(int character) const noexcept;
};

#else

class Font {
public:
	Font(lsd::StringView texturePath, glm::uvec2 charSize, std::uint32_t padding) noexcept;

	[[nodiscard]] RenderSystem::CallData drawCall(const glm::mat4& transform, const TextBox& textBox) const;

private:
	Texture m_texture;

	glm::ivec2 m_charSize;
	std::int32_t m_padding;

	[[nodiscard]] SDL_Rect at(int c) const noexcept;
};

#endif

} // namespace esengine
