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

namespace esengine {

class Camera {
public:
	Camera(lsd::StringView passName = { }) : m_passName(passName) { }

	[[nodiscard]] const lsd::String& passName() const noexcept {
		return m_passName;
	}

private:
	lsd::String m_passName;
};

} // namespace esengine
