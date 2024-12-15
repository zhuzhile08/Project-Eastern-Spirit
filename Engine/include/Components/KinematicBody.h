/************************
 * @file KinematicBody.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Kinematic physics body
 * 
 * @date 2024-11-26
 * @copyright Copyright (c) 2024
 ************************/

#pragma once

#include <Core/Common.h>
#include <Core/Math.h>

#include <ETCS/Components/Transform.h>

#include <glm/glm.hpp>

namespace esengine {

class KinematicBody {
public:
	KinematicBody(glm::vec2 collider, glm::vec2 offset, std::size_t layers = 1) : 
		m_collider(collider), m_offset(offset), m_layers(layers) { }

	void move(etcs::Transform& transform);

	[[nodiscard]] BoundingBox aabb(const etcs::Transform& transform) const noexcept;

	[[nodiscard]] glm::vec2 collider() const noexcept {
		return m_collider;
	}
	[[nodiscard]] glm::vec2 offset() const noexcept {
		return m_offset;
	}
	[[nodiscard]] std::size_t layers() const noexcept {
		return m_layers;
	}
	[[nodiscard]] bool collided() const noexcept {
		return m_collided;
	}
	[[nodiscard]] std::uint32_t collidedLayers() const noexcept {
		return m_collidedLayers;
	}

	glm::vec2 velocity;

private:
	glm::vec2 m_collider;
	glm::vec2 m_offset;

	std::uint32_t m_layers;

	bool m_collided;
	std::uint32_t m_collidedLayers;

	friend class detail::PhysicsSystem;
};

} // namespace esengine
