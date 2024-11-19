#include <Components/Sprite.h>

#include <Components/Camera.h>

#include <Graphics/Texture.h>
#include <Graphics/Window.h>

namespace esengine {

bool Sprite::drawCall(
	RenderSystem::CallData& data,
	const etcs::Entity& entity,
	const etcs::Transform& transform,
	const glm::mat4& camTransform,
	const Camera& camera) const {
	auto tf = camTransform * glm::vec4(transform.globalTranslation(entity), 1.0f);

	if (tf.x >= -m_rect.w && tf.x < camera.viewport.w && tf.y >= -m_rect.h && tf.y < camera.viewport.h) {
		auto scale = transform.globalScale(entity) * glm::vec3(m_rect.w, m_rect.h, 1.f);

		data = {
			tf.z,
			transform.globalOrientation(entity).z * -360,
			m_rect,
			SDL_FRect {
				tf.x + camera.viewport.x,
				tf.y + camera.viewport.y,
				scale.x,
				scale.y
			},
			m_texture->texture(camera.passName())
		};

		return true;
	} else return false;
}

} // namespace esengine
