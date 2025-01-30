/*************************
 * @file Collision.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Basic core definitions used for the collision system
 * 
 * @date 2025-01-28
 * @copyright Copyright (c) 2025
 *************************/

#pragma once

#include <Core/Common.h>
#ifndef NDEBUG
#include <Detail/RenderSystem.h>
#endif

#include <ETCS/Components/Transform.h>

#include <glm/glm.hpp>

namespace esengine {

// Core definitions

using collision_layer_t = std::uint64_t;

enum class Collision {
	none,
	up = 1 << 0,
	down = 1 << 1,
	left = 1 << 2,
	right = 1 << 3
};

namespace detail {

// Bounding box

struct BoundingBox {
	glm::vec2 min;
	glm::vec2 max;
};


// Base class for all collider structures

class BasicCollider {
public:
	BasicCollider(const glm::vec2& collider, const glm::vec2& offset, std::size_t layers = 1) : 
		collider(collider), offset(offset), layers(layers) { }

#ifndef NDEBUG
	void debugDrawCall(detail::RenderSystem::CallData& data,
		const etcs::Entity& entity,
		const etcs::Transform& transform,
		const glm::mat4& camTransform,
		const Camera& camera);
#endif

	[[nodiscard]] BoundingBox aabb(const etcs::Transform& transform) const noexcept;

	[[nodiscard]] Collision collisions() const noexcept {
		return m_collisions;
	}
	[[nodiscard]] std::uint64_t collidedLayers() const noexcept {
		return m_collidedLayers;
	}

	glm::vec2 collider;
	glm::vec2 offset;

	std::uint64_t layers;

protected:
	Collision m_collisions;
	std::uint64_t m_collidedLayers;

	void clearCollisions() noexcept;
};

} // namespace detail


// Collider implementations

class StaticBody : public detail::BasicCollider {
public:
	using detail::BasicCollider::BasicCollider;

	friend class detail::PhysicsSystem;
};

class KinematicBody : public detail::BasicCollider {
public:
	using detail::BasicCollider::BasicCollider;

	void move(etcs::Transform& transform);

	glm::vec2 velocity;

	friend class detail::PhysicsSystem;
};

class AreaBody : public detail::BasicCollider {
	using detail::BasicCollider::BasicCollider;

	friend class detail::PhysicsSystem;
};

}
