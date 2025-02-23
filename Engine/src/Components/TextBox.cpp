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
	if (m_text.size() == 0 || m_renderLimit == 0) return;

	auto tf = renderMatrix * glm::vec4(transform.globalTranslation(entity), 1.0f) + glm::vec4(m_offset, 0.0f, 0.0f);

	if (m_dirty) {
		m_textDim = { 0, 1 };
		m_textPixelDim = { };
		for (std::size_t i = 0; i < std::min(m_text.size(), m_renderLimit); i++) {
			const auto& c = m_font->at(m_text[i]);

			if (auto width = m_textPixelDim.x + c.w + m_padding.x; width > m_dimension.x) {
				if (auto height = m_textPixelDim.y + c.h + m_padding.y; height <= m_dimension.y) {
					m_textPixelDim.y = height;
					++m_textDim.y;
				} else break;
			} else {
				m_textPixelDim.x = width;
				++m_textDim.x;
			}
		}
	}

	float startingXPos { };
	if (m_horizontalAlignment == Alignment::center) startingXPos = tf.x + (m_dimension.x - m_textPixelDim.x) / 2.0f;
	else if (m_horizontalAlignment == Alignment::right) startingXPos = tf.x + m_dimension.x - m_textPixelDim.x;
	else startingXPos = tf.x;

	float height = m_font->cellSize().y - m_font->capLine() - m_font->baseLine();

	auto tx = m_font->texture().texture();
	auto& drawCalls = globals::renderSystem->drawData(static_cast<int>(tf.z));

	auto textIt = m_text.begin();
	for (auto y = 0; y < m_textDim.y; y++) {
		float width = 0;

		for (; m_textDim.x > 0; m_textDim.x--) {
			const auto& c = m_font->at(*textIt);

			drawCalls.emplaceBack(
				0.0f,
				c,
				SDL_FRect {
					startingXPos + width,
					tf.y + (y + m_padding.y) * height,
					c.w,
					c.h
				},
				tx
			);

			width += c.w + m_padding.x;
			++textIt;
		}
	}
}

} //namespace esengine