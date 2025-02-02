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

namespace constants {

// epsilons

inline constexpr float fepsilon = std::numeric_limits<float>::epsilon();
inline constexpr double depsilon = std::numeric_limits<double>::epsilon();

// infinities

inline constexpr float finfinity = std::numeric_limits<float>::infinity();
inline constexpr double dinfinity = std::numeric_limits<double>::infinity();

// nans

inline constexpr float fnan = std::numeric_limits<float>::quiet_NaN();
inline constexpr double dnan = std::numeric_limits<double>::quiet_NaN();

}


// interpolation

inline GLM_CONSTEXPR glm::vec2 interpolateLinear(glm::vec2 a, glm::vec2 b, float t) noexcept {
	return a * (1.0f - t) + b * t;
}
inline GLM_CONSTEXPR glm::dvec2 interpolateLinear(glm::dvec2 a, glm::dvec2 b, double t) noexcept {
	return a * (1.0f - t) + b * t;
}

inline constexpr float interpolateLinear(float a, float b, float t) noexcept {
	return a * (1.0f - t) + b * t;
}
inline constexpr float interpolateLinear(double a, double b, double t) noexcept {
	return a * (1.0f - t) + b * t;
}


inline GLM_CONSTEXPR glm::ivec2 mapToQuad(glm::ivec2 quadPos, glm::ivec2 quadDim, glm::ivec2 pos) noexcept {
	return glm::ivec2((pos.x + quadPos.x) % quadDim.x, (pos.y + quadPos.y) % quadDim.y);
}


template <class Numerical> inline GLM_CONSTEXPR Numerical closestToZero(Numerical a, std::type_identity_t<Numerical> b) noexcept requires std::is_arithmetic_v<Numerical> {
	return ((glm::abs(a) > glm::abs(b)) ? b : a);
}

template <class FloatingPoint> inline constexpr bool fpEqual(FloatingPoint a, std::type_identity_t<FloatingPoint> b) noexcept requires std::is_floating_point_v<FloatingPoint> {
	auto diff = a - b;
	return (diff < constants::fepsilon) && (-diff < constants::depsilon);
}

} // namespace esengine
