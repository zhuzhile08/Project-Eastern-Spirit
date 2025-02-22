#include <Components/TextBox.h>

#include <Components/Camera.h>

#include <Graphics/Texture.h>
#include <Graphics/Window.h>

#include <Detail/RenderSystem.h>

namespace esengine {

void TextBox::draw(
	const etcs::Entity& entity,
	const etcs::Transform& transform,
	const glm::mat4& renderMatrix
) const {
	auto tf = renderMatrix * glm::vec4(transform.globalTranslation(entity), 1.0f) + glm::vec4(m_offset, 0.0f, 0.0f);

	glm::ivec2 textDim { };
	glm::ivec2 textPixelDim { };
	for (std::size_t i = 0; i < std::min(text.size(), m_index); i++) {
		const auto& c = m_font->at(text[i]);

		if (auto width = textPixelDim.x + c.w + m_padding.x; width > m_dimension.x) {
			if (auto height = textPixelDim.y + c.h + m_padding.y; height <= m_dimension.y) {
				textPixelDim.y = height;
				++textDim.y;
			} else break;
		} else {
			textPixelDim.x = width;
			++textDim.x;
		}
	}

	float startingXPos { };
	if (m_horizontalAlignment == Alignment::center) startingXPos = tf.x + textPixelDim.x * 2.0f;
	else if (m_horizontalAlignment == Alignment::right) startingXPos = tf.x + textPixelDim.x;
	else startingXPos = tf.x;

	auto tx = m_font->texture().texture();
	auto& drawCalls = globals::renderSystem->drawData(static_cast<int>(tf.z));

	for (auto y = 0; y < textDim.y; y++) {
		for (auto x = 0; x < textDim.x; x++) {
			const auto& c = m_font->at(text[x + y * textDim.x]);

			drawCalls.emplaceBack(
				0.0f,
				c,
				SDL_FRect {
					startingXPos + x * c.w + m_padding.x,
					tf.y + y * c.h + m_padding.y,
					c.w,
					c.h
				},
				tx
			);
		}
	}
}

} //namespace esengine