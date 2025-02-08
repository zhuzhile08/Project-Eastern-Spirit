#include <Components/Sprite.h>

#include <Components/Camera.h>

#include <Graphics/Texture.h>
#include <Graphics/Window.h>

namespace esengine {

void Sprite::draw(const etcs::Entity& entity, const etcs::Transform& transform, const glm::mat4& renderMatrix) const {
	auto tf = renderMatrix * glm::vec4(transform.globalTranslation(entity), 1.0f);

	auto scale = transform.globalScale(entity) * glm::vec3(m_rect.w, m_rect.h, 1.f);

	globals::renderSystem->drawData()[static_cast<std::size_t>(tf.z) * constants::depthSortingFactor].emplaceBack(
		transform.globalOrientation(entity).z * -360,
		m_rect,
		SDL_FRect {
			tf.x,
			tf.y,
			scale.x,
			scale.y
		},
		m_texture->texture()
	);
}

} // namespace esengine
