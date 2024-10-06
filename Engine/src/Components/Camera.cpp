#include <Components/Camera.h>

#include <Graphics/Window.h>

#include <glm/glm.hpp>

namespace esengine {

Camera::Camera(lsd::StringView passName, double fov, double near) : m_passName(passName), m_projectionMat(0.0f), m_fov(fov), m_near(near) {
	double f = glm::tan(glm::radians(fov / 2));
	double a = lsd::implicitCast<double>(globals::window->size().x) / globals::window->size().y;

	m_projectionMat[0][0] = 1.0f / (f * a);
	m_projectionMat[1][1] = 1.0f;
	m_projectionMat[1][3] = 1.0f;
	m_projectionMat[2][2] = 1.0f / f;
	m_projectionMat[3][3] = near;
}

void Camera::recalculate(double fov, double near) {
	if (fov != 0) {
		m_fov = fov;

		double f = glm::tan(glm::radians(fov / 2));
		double a = lsd::implicitCast<double>(globals::window->size().x) / globals::window->size().y;

		m_projectionMat[0][0] = 1.0f / (f * a);
		m_projectionMat[2][2] = 1.0f / f;
	}

	if (near != 0) {
		m_near = near;

		m_projectionMat[3][3] = near;
	}
}
void Camera::update() {
	if (globals::window->changed()) 
		m_projectionMat[0][0] = m_projectionMat[2][2] / (lsd::implicitCast<double>(globals::window->size().x) / globals::window->size().y);
}

} // esengine
