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

#include <cstddef>

namespace esengine {
class Rect {
public:
	Rect(const glm::uvec2& pos, const glm::uvec2& rec) noexcept :
		m_pos(pos),
		m_rec(rec) { }

	[[nodiscard]] const glm::uvec2& pos() const noexcept {
		return m_pos;
	}
	[[nodiscard]] const glm::uvec2& rec() const noexcept {
		return m_rec;
	}

private:
	glm::uvec2 m_pos;
	glm::uvec2 m_rec;
};
class TextBox : public Rect  {
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

	TextBox(lsd::StringView text, const Font& font, const glm::uvec2& kerning, const glm::uvec2& pos, const glm::uvec2& rec, Alignment alignment = Alignment::def, bool scrolingAnimation = false) :
		Rect(pos, rec),
		m_text(text),
		m_font(font),
		m_kerning(kerning),
		m_alignment(alignment),
		m_scrolingAnimation(scrolingAnimation) { }
		
	void changeText(lsd::StringView newText);

private:
	glm::uvec2 m_kerning;
	lsd::String m_text;

	Alignment m_alignment;
	bool m_scrolingAnimation;

	const Font& m_font;
	
};


} // namespace esengine
