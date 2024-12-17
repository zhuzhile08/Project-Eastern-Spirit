#include <Components/Camera.h>

#include <Graphics/Window.h>

#include <glm/glm.hpp>

namespace esengine {

#ifdef ESENGINE_DYNAMIC_WINDOW_SIZE
Camera::Camera(lsd::StringView passName) : 
	viewport { 
		0.f, 
		0.f, 
		lsd::implicitCast<float>(globals::window->size().x), 
		lsd::implicitCast<float>(globals::window->size().y) 
	}, m_passName(passName) { }
#else
Camera::Camera(lsd::StringView passName) : 
	viewport { 
		0.f, 
		0.f, 
		lsd::implicitCast<float>(globals::window->baseSize().x), 
		lsd::implicitCast<float>(globals::window->baseSize().y) 
	}, m_passName(passName) { }
#endif

Camera::Camera(SDL_FRect viewport, lsd::StringView passName) : viewport(viewport), m_passName(passName) { }

glm::mat4 Camera::transformMat(const etcs::Entity& camera, const etcs::Transform& transform) const noexcept {
	/*
	 * This matrix just transforms the object into the local coordinates of the viewport
	 * The results are in pixel coordinates and are henceforth not normalized
	 */
	glm::mat4 mat(0.0f);

	mat[0][0] = zoom.x;
	mat[1][1] = zoom.y;
	mat[2][2] = 1;
	mat[3][3] = 1;

	if (target.valid()) {
		mat[3][0] = zoom.x * (-transform.translation().x + offset.x - viewport.x);
		mat[3][1] = zoom.y * (-transform.translation().y + offset.y - viewport.y);
		mat[3][2] = zoom.y * (-transform.translation().y);
	} else {
		mat[3][0] = zoom.x * (-transform.globalTranslation(camera).x + offset.x - viewport.x);
		mat[3][1] = zoom.y * (-transform.globalTranslation(camera).y + offset.y - viewport.y);
		mat[3][2] = zoom.y * (-transform.globalTranslation(camera).y);
	}

	return mat;
}

void Camera::update(etcs::Transform& transform) {
	if (target.valid()) {
		transform.translation().x = std::clamp(transform.translation().x, limits.x, limits.x + limits.w);
		transform.translation().y = std::clamp(transform.translation().y, limits.y, limits.y - limits.y);
	}
}

} // namespace esengine
