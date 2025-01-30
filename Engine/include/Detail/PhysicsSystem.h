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

#include <Components/Colliders.h>

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

	struct AreaEntity {
		etcs::Transform* transform;
		AreaBody* body;
		BoundingBox aabb;
	};

public:
	void update();

private:
	lsd::Vector<KinematicEntity> m_kinematics;
	lsd::Vector<StaticEntity> m_statics;
	lsd::Vector<AreaEntity> m_areas;

	void handleKinematicCollision(KinematicEntity& first, KinematicEntity& second, std::uint64_t layers) const;
	void handleStaticCollision(KinematicEntity& kinematicEntity, StaticEntity& staticEntity, std::uint64_t layers) const;
	void handleAreaCollision(KinematicEntity& kinematicEntity, AreaEntity& areaEntity, std::uint64_t layers) const;

	void setCollisionFlags(const glm::vec2& timeToCol, const glm::vec2& firstVel, Collision& first, Collision& second) const noexcept;
};

} // namespace detail

} // namespace esengine
