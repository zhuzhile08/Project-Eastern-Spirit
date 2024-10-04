#include <Components/Sprite3D.h>

#include <Components/Camera.h>

#include <Graphics/Texture.h>
#include <Graphics/Window.h>

namespace esengine {

RenderSystem::CallData Sprite3D::drawCall(const glm::vec3& translation, const glm::mat4& camTransform, const Camera& camera) const {
	auto transform = camera.projectionMat() * camTransform * glm::vec4(translation.x, translation.y, translation.z, 1.0f); // global transformation
	auto scale = camera.projectionMat() * glm::vec4(m_rect.w / globals::spriteUnit, translation.y, m_rect.w / globals::spriteUnit, 1.0f);

	((transform.x /= transform.w) += 1.0f) *= 0.5f * esengine::globals::window->size().x;
	((transform.z /= transform.w) += 1.0f) *= 0.5f * esengine::globals::window->size().y;

	(scale.x /= scale.w) *= esengine::globals::window->size().x;
	(scale.z /= scale.w) *= esengine::globals::window->size().y;

	return {
		0.0f, /// @todo camera rotation
		m_rect,
		SDL_FRect {
			transform.x,
			transform.z,
			scale.x,
			scale.z
		},
		m_texture->texture(camera.passName()),
		camera.passName()
	};
}

} // namespace esengine
