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

#include <limits>

namespace esengine {

inline GLM_CONSTEXPR glm::vec2 interpolateLinear(glm::vec2 a, glm::vec2 b, float t) noexcept {
	return a * (1.0f - t) + b * t;
}
inline GLM_CONSTEXPR glm::dvec2 interpolateLinear(glm::dvec2 a, glm::dvec2 b, double t) noexcept {
	return a * (1.0f - t) + b * t;
}

/*
inline constexpr float interpolateLinear(float low, float high, float t) noexcept {
	//return a * (1.0f - t) + b * t;
}
inline constexpr float interpolateLinear(double low, double high, double t) noexcept {
	//return a * (1.0f - t) + b * t;
}
*/

inline GLM_CONSTEXPR glm::ivec2 mapToQuad(glm::ivec2 quadPos, glm::ivec2 quadDim, glm::ivec2 pos) noexcept {
	return glm::ivec2((pos.x + quadPos.x) % quadDim.x, (pos.y + quadPos.y) % quadDim.y);
}

template <class FloatingPoint> inline constexpr bool fpEqual(FloatingPoint a, FloatingPoint b) noexcept requires std::is_floating_point_v<FloatingPoint> {
	auto diff = a - b;
	return (diff < std::numeric_limits<FloatingPoint>::epsilon()) && (-diff < std::numeric_limits<FloatingPoint>::epsilon());
}

} // namespace esengine
