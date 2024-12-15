/***********************
 * @file PhysicsSystem.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief System which manages collision events
 * 
 * @date 2024-11-30
 * @copyright Copyright (c) 2024
 ***********************/

#pragma once

#include <Core/Common.h>

#include <Components/KinematicBody.h>
#include <Components/StaticBody.h>

#include <ETCS/Components/Transform.h>

#include <LSD/Vector.h>
#include <LSD/UnorderedSparseMap.h>

#include <utility>

namespace esengine {

namespace detail {

class PhysicsSystem {
private:
	struct KinematicEntity {
		etcs::Transform* transform;
		KinematicBody* body;
		BoundingBox aabb;
	};

	struct StaticEntity {
		etcs::Transform* transform;
		StaticBody* body;
		BoundingBox aabb;
	};

	struct Manifest {
		std::size_t layers;
		glm::vec2 penetrationDirection { 1.0f, 1.0f };
	};

public:
	void update();

private:
	lsd::Vector<KinematicEntity> m_kinematics;
	lsd::Vector<StaticEntity> m_statics;

	bool aabbCollided(
		Manifest& manifest, 
		BoundingBox& first, 
		const glm::vec2& vFirst, 
		BoundingBox& second,
		const glm::vec2& vSecond) const;

	void handleKinematicCollision(const Manifest& manifest, KinematicEntity& first, KinematicEntity& second) const;
	void handleStaticCollision(const Manifest& manifest, KinematicEntity& kinematicEntity, StaticEntity& staticEntity) const;
};

} // namespace detail

} // namespace esengine
