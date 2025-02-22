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

#include <Core/Animation.h>

#include <Graphics/Font.h>
#include <Graphics/Texture.h>

#include <LSD/String.h>
#include <LSD/StringView.h>

#include <ETCS/Component.h>
#include <ETCS/Components/Transform.h>

#include <SDL3/SDL.h>

#include <glm/glm.hpp>

#include <cstddef>
#include <limits>

namespace esengine {

class TextBox {
public:
	enum class Alignment {
		center = 1,
		left = 2,
		right = 3,
		def = left
	};

	TextBox(
		lsd::StringView text, 
		Font* font, 
		const glm::vec2& dimension, 
		const glm::vec2& offset = { 0.0f, 0.0f },
		const glm::vec2& padding = { 0.0f, 0.0f },
		Alignment horizontalAlignment = Alignment::def, 
		bool clipping = true
	) : text(text),
		m_offset(offset),
		m_dimension(dimension),
		m_padding(padding),
		m_horizontalAlignment(horizontalAlignment),
		m_clipping(clipping),
		m_font(font) { }


	void draw(const etcs::Entity& entity, const etcs::Transform& transform, const glm::mat4& renderMatrix) const;
		
	
	[[nodiscard]] const glm::vec2& offset() const noexcept {
		return m_offset;
	}
	[[nodiscard]] const glm::vec2& dimension() const noexcept {
		return m_dimension;
	}
	[[nodiscard]] const glm::vec2& padding() const noexcept {
		return m_padding;
	}
	[[nodiscard]] Alignment horizontalAlignment() const noexcept {
		return m_horizontalAlignment;
	}
	[[nodiscard]] const Font* font() const noexcept {
		return m_font;
	}

	lsd::String text;

private:
	glm::vec2 m_offset;
	glm::vec2 m_dimension;

	glm::vec2 m_padding;

	Alignment m_horizontalAlignment;
	bool m_clipping;

	std::size_t m_index = std::numeric_limits<std::size_t>::max();

	Font* m_font;

	friend class TextBoxAnimator;
};


} // namespace esengine
