/************************
 * @file Camera.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief 
 * 
 * @date 2024-09-10
 * 
 * @copyright Copyright (c) 2024
 ************************/

#pragma once

#include <ETCS/Component.h>
#include <ETCS/Components/Transform.h>

#include <LSD/String.h>

#include <Common/Common.h>

namespace esengine {

class Camera {
public:
	Camera(lsd::StringView passName = { }, double fov = 90, double near = 0.1f);

	void recalculate(double fov = 0, double near = 0);
	void update();

	[[nodiscard]] const glm::mat4& projectionMat() const noexcept {
		return m_projectionMat;
	}
	[[nodiscard]] const lsd::String& passName() const noexcept {
		return m_passName;
	}
	[[nodiscard]] double near() const noexcept {
		return m_near;
	}
	[[nodiscard]] double fov() const noexcept {
		return m_fov;
	}

private:
	lsd::String m_passName;

	glm::mat4 m_projectionMat;

	double m_fov;
	double m_near;
};

} // namespace esengine
