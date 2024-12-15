/*************************
 * @file Font.h
 * @author Okeeee_Holz
 *
 * @brief TextBox
 *
 * @date 2024-11-17
 *
 * @copyright Copyright (c) 2024
 *************************/

#pragma once

#include <Graphics/Font.h>
#include <Graphics/Texture.h>

#include <LSD/String.h>
#include <LSD/StringView.h>

#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include <ETCS/Component.h>
#include <ETCS/Components/Transform.h>

#include <cstddef>

namespace esengine {

class TextBox {
public:
	enum class Alignment {
		center = 1,
		left = 2,
		right = 3,
		top = 8,
		middle = 16,
		bottom = 24,
		def = left | middle
		
	};


	TextBox(
		lsd::StringView text, 
		const Font& font, 
		const glm::uvec2& kerning, 
		const glm::vec2& offset, 
		const glm::uvec2& dimension, 
		Alignment alignment = Alignment::def, 
		bool scrolling = false
	) : text(text),
		scrolling(scrolling),
		m_offset(offset),
		m_dimension(dimension),
		m_kerning(kerning),
		m_alignment(alignment),
		m_font(font) { }


	void draw(
		const etcs::Entity& entity,
		const etcs::Transform& transform,
		const glm::mat4& camTransform,
		const Camera& camera) const;
		
	
	[[nodiscard]] const glm::vec2& offset() const noexcept {
		return m_offset;
	}
	[[nodiscard]] const glm::uvec2& dimension() const noexcept {
		return m_dimension;
	}
	[[nodiscard]] const glm::uvec2& kerning() const noexcept {
		return m_kerning;
	}
	[[nodiscard]] Alignment alignment() const noexcept {
		return m_alignment;
	}
	[[nodiscard]] const Font& font() const noexcept {
		return m_font;
	}


	lsd::String text;
	bool scrolling;

private:
	glm::vec2 m_offset;
	glm::uvec2 m_dimension;

	glm::uvec2 m_kerning;

	Alignment m_alignment;

	const Font& m_font;
};


} // namespace esengine
