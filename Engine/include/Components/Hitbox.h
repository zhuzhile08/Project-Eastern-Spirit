/*************************
 * @file Hitbox.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Hitboxes which define a box in which hurtboxes can be detected
 * 
 * @date 2024-10-02
 * @copyright Copyright (c) 2024
 *************************/

#pragma once

#include <glm/glm.hpp>

namespace esengine {

class Hitbox {
public:
	constexpr Hitbox(glm::vec3 dimensions) : m_dimensions(dimensions) { }

private:
	glm::vec3 m_dimensions;
};

} // namespace esengine
