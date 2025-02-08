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

	auto charSize = glm::vec2(m_font->charSize());
	auto maxWidth = static_cast<int>(m_dimension.x / charSize.x) + 1;
	auto renderIndex = std::min(text.size(), m_index);

	glm::ivec2 textDim {
		renderIndex % maxWidth,
		static_cast<int>(glm::ceil(glm::min(renderIndex / lsd::implicitCast<float>(maxWidth), m_dimension.y / charSize.y)))
	};

	float startingXPos { };
	if (m_horizontalAlignment == Alignment::center) startingXPos = tf.x + (maxWidth - textDim.x) * charSize.x / 2.0f;
	else if (m_horizontalAlignment == Alignment::right) startingXPos = tf.x + (maxWidth - textDim.x) * charSize.x;
	else startingXPos = tf.x;

	auto tx = m_font->texture().texture();
	auto& drawCalls = globals::renderSystem->drawData()[static_cast<int>(tf.z)];

	for (auto y = 0; y < textDim.y; y++) {
		for (auto x = 0; x < textDim.x; x++) {
			drawCalls.emplaceBack(
				0.0f,
				m_font->at(text[x + y * textDim.x]),
				SDL_FRect {
					startingXPos + x * charSize.x,
					tf.y + y * charSize.y,
					charSize.x,
					charSize.y
				},
				tx
			);
		}
	}
}

} //namespace esengine