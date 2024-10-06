/*************************
 * @file Math.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Common math functions
 * 
 * @date 2024-10-06
 * @copyright Copyright (c) 2024
 *************************/

#pragma once

#include <glm/glm.hpp>

namespace esengine {

inline GLM_CONSTEXPR glm::vec2 interpolateLinear(glm::vec2 a, glm::vec2 b, float t) noexcept {
	return a * (1.0f - t) + b * t;
}
inline GLM_CONSTEXPR glm::dvec2 interpolateLinear(glm::dvec2 a, glm::dvec2 b, double t) noexcept {
	return a * (1.0f - t) + b * t;
}

inline GLM_CONSTEXPR glm::vec2 mapToQuad(glm::ivec2 quadPos, glm::ivec2 quadDim, glm::ivec2 pos) noexcept {
	return glm::vec2((pos.x + quadPos.x) % quadDim.x, (pos.y + quadPos.y) % quadDim.y);
}

} // namespace esengine
