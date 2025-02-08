#include <Components/Camera.h>

#include <Graphics/Window.h>

#include <ETCS/Entity.h>

#include <glm/glm.hpp>

namespace esengine {

Camera::Camera(lsd::StringView passName, SDL_Texture* target) : m_passName(passName) {
	globals::renderSystem->insertPass(passName, target, { 0, 0, 1, 1 }, { 0, 0, 1, 1 });
}

Camera::Camera(const SDL_FRect& viewport, lsd::StringView passName, SDL_Texture* target) : m_passName(passName) {
	globals::renderSystem->insertPass(passName, target, viewport, { 0, 0, 1, 1 });
}

void Camera::destoryRenderPass() const {
	globals::renderSystem->removePass(m_passName);
}

void Camera::setRenderPassClipRect(const SDL_FRect& clipRect) const {
	globals::renderSystem->pass(m_passName).clipRect = clipRect;
}

void Camera::setRenderPassViewport(const SDL_FRect& viewport) const {
	globals::renderSystem->pass(m_passName).viewport = viewport;
}

SDL_FRect Camera::renderPassClipRect() const {
	return globals::renderSystem->pass(m_passName).clipRect;
}

SDL_FRect Camera::renderPassViewport() const {
	return globals::renderSystem->pass(m_passName).viewport;
}

glm::mat4 Camera::renderMatrix(const etcs::Entity& entity, etcs::Transform& transform) {
	/*
	 * This matrix just transforms the object into the local coordinates of the viewport
	 * The results are in pixel coordinates and are henceforth not normalized
	 */
	glm::mat4 mat(0.0f);

	mat[0][0] = zoom.x;
	mat[1][1] = zoom.y;
	mat[2][2] = 1;
	mat[3][3] = 1;

	glm::vec3 translation;

	if (target.alive() && target.contains<etcs::Transform>()) {
		translation = target.component<etcs::Transform>().get().globalTranslation(target);
	} else {
		translation = transform.globalTranslation(entity);
	}

	mat[3][0] = zoom.x * (-translation.x + offset.x);
	mat[3][1] = zoom.y * (-translation.y + offset.y);
	mat[3][2] = zoom.y * (-translation.y);
	
	return mat;
}

bool Camera::validRenderPass() const noexcept {
	return globals::renderSystem->containsPass(m_passName);
}

} // namespace esengine
