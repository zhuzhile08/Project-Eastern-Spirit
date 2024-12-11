/************************
 * @file StaticBody.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Static physics-bodies which only influence others, and can't be moved except manually or influenced
 * 
 * @date 2024-12-08
 * @copyright Copyright (c) 2024
 ************************/

#pragma once

#include <Core/Common.h>
#include <Core/Math.h>

#include <ETCS/Components/Transform.h>

#include <glm/glm.hpp>

namespace esengine {

class StaticBody {
public:
	StaticBody(glm::vec2 collider, glm::vec2 offset, std::size_t layers = 0, std::size_t detectionLayers = 0) : 
		m_collider(collider), m_offset(offset), m_layers(layers), m_detectionLayers(detectionLayers) { }

	[[nodiscard]] BoundingBox aabb(const etcs::Transform& transform) const noexcept;

	[[nodiscard]] glm::vec2 offset() const noexcept {
		return m_offset;
	}
	[[nodiscard]] std::size_t layers() const noexcept {
		return m_layers;
	}
	[[nodiscard]] std::size_t detectionLayers() const noexcept {
		return m_detectionLayers;
	}
	[[nodiscard]] bool collided() const noexcept {
		return m_collided;
	}
	[[nodiscard]] std::uint32_t detectedLayer() const noexcept {
		return m_detectedLayer;
	}

private:
	glm::vec2 m_collider;
	glm::vec2 m_offset;

	std::uint32_t m_layers;
	std::uint32_t m_detectionLayers;

	bool m_collided;
	std::uint32_t m_detectedLayer;

	friend class detail::PhysicsSystem;
};

} // namespace esengine
