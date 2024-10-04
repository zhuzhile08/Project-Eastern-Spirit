/*************************
 * @file Hurtbox.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Hurtboxes which define a box that can be detected by a hurtbox
 * 
 * @date 2024-10-02
 * @copyright Copyright (c) 2024
 *************************/

#pragma once

#include <glm/glm.hpp>

namespace esengine {

class Hurtbox {
public:
	constexpr Hurtbox(glm::vec3 dimensions) : m_dimensions(dimensions) { }

private:
	glm::vec3 m_dimensions;
};

} // namespace esengine
