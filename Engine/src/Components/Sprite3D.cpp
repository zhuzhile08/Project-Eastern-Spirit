#include <Components/Sprite3D.h>

#include <Components/Camera.h>

#include <Graphics/Texture.h>
#include <Graphics/Window.h>

namespace esengine {

static constexpr std::size_t fov = 100;
static const float fovRatio = glm::tan(glm::radians(static_cast<float>(fov / 2)));

RenderSystem::CallData Sprite3D::drawCall(const glm::vec3& translation, const Camera& camera) const {
	auto tf = glm::vec4(translation, 1.0f); // camTransform*

	float fovRatY = tf.y * fovRatio;

	float px = tf.x / fovRatY * esengine::globals::window->size().x + esengine::globals::window->size().x / 2; // perspective calculations
	float py = -tf.z / fovRatY * esengine::globals::window->size().y + esengine::globals::window->size().y / 2;
	float w = esengine::globals::window->size().x / fovRatY;

	return {
		0.0f, // temporary
		m_rect,
		SDL_FRect {
			(px - w / 2),
			(py - w / 2),
			w,
			w
		},
		m_texture,
		camera.passName()
	};
}

} // namespace esengine
