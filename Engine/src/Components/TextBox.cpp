#include <Components/TextBox.h>

#include <Components/Camera.h>

#include <Graphics/Texture.h>
#include <Graphics/Window.h>

#include <Detail/RenderSystem.h>

namespace esengine {

void TextBox::draw(
	const etcs::Entity& entity,
	const etcs::Transform& transform,
	const glm::mat4& camTransform,
	const Camera& camera) const {
	auto tf = camTransform * glm::vec4(transform.globalTranslation(entity), 1.0f) + glm::vec4(m_offset, 0.0f, 0.0f);

	if (tf.x >= -m_dimension.x && tf.x < camera.viewport.w && tf.y >= -m_dimension.y && tf.y < camera.viewport.h) {
		auto& renderPass = globals::renderSystem->pass(camera.passName());
		auto& drawCalls = renderPass.drawData[static_cast<int>(tf.z * renderPass.sortingFactor)];

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

		for (auto y = 0; y < textDim.y; y++) {
			for (auto x = 0; x < textDim.x; x++) {
				drawCalls.emplaceBack(
					0.0f, // not needed, if there are changes to the sorting system, change to tf.z
					0.0f,
					m_font->at(text[x + y * textDim.x]),
					SDL_FRect {
						startingXPos + x * charSize.x,
						tf.y + y * charSize.y,
						charSize.x,
						charSize.y
					},
					m_font->texture()
				);
			}
		}
	}
}

} //namespace esengine