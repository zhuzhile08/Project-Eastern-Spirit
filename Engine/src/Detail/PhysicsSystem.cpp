#include <Detail/PhysicsSystem.h>

#include <Core/Math.h>

#include <ETCS/ETCS.h>

#include <LSD/Operators.h>

using namespace lsd::enum_operators;

namespace esengine {

namespace globals {

detail::PhysicsSystem* physicsSystem = nullptr;

}

namespace detail {

void PhysicsSystem::update() {
	for (auto [entity, transform, body] : etcs::world().query<const etcs::Entity, etcs::Transform, KinematicBody>()) {
		body.clearCollisions();
		m_kinematics.emplaceBack(&transform, &body, body.aabb(entity, transform));
	}
	
	for (auto [entity, transform, body] : etcs::world().query<const etcs::Entity, etcs::Transform, StaticBody>()) {
		body.clearCollisions();
		m_statics.emplaceBack(&transform, &body, body.aabb(entity, transform));
	}
	
	for (auto [entity, transform, body] : etcs::world().query<const etcs::Entity, etcs::Transform, AreaCollider>()) {
		body.clearCollisions();
		m_areas.emplaceBack(&transform, &body, body.aabb(entity, transform));
	}

	for (auto kinematicIt = m_kinematics.begin(); kinematicIt != m_kinematics.end(); kinematicIt++) {
		auto currLayer = kinematicIt->body->layers; // Should be somewhat of an improvement

		for (auto secondIt = kinematicIt + 1; secondIt != m_kinematics.end(); secondIt++) // Avoid duplicate collisions and collisions with self
			if (auto layers = currLayer & secondIt->body->layers; layers != 0)
				handleKinematicCollision(*kinematicIt, *secondIt, layers);

		for (auto& staticEntity : m_statics)
			if (auto layers = currLayer & staticEntity.body->layers; layers != 0)
				handleStaticCollision(*kinematicIt, staticEntity, layers);
		
		for (auto& areaEntity : m_areas)
			if (auto layers = currLayer & areaEntity.body->layers; layers != 0)
				handleAreaCollision(*kinematicIt, areaEntity, layers);
	}

	// Apply velocity to kinematic objects
	for (auto& [transform, kinematicBody, _] : m_kinematics) {
		kinematicBody->move(*transform);
	}

	// Clear all data from the frame
	m_kinematics.clear();
	m_statics.clear();
}

void PhysicsSystem::handleKinematicCollision(KinematicEntity& first, KinematicEntity& second, std::uint64_t layers) const {
	auto& vFirst = first.body->velocity;
	auto& vSecond = second.body->velocity;

	auto firstTranslated = BoundingBox { first.aabb.min + vFirst, first.aabb.max + vFirst };
	auto secondTranslated = BoundingBox { second.aabb.min + vSecond, second.aabb.max + vSecond };

	if (firstTranslated.max.x > secondTranslated.min.x && 
		firstTranslated.min.x < secondTranslated.max.x && 
		firstTranslated.max.y > secondTranslated.min.y && 
		firstTranslated.min.y < secondTranslated.max.y
	) {
		auto distance = glm::vec2 {
			glm::max(first.aabb.min.x, second.aabb.min.x) - glm::min(first.aabb.max.x, second.aabb.max.x),
			glm::max(first.aabb.min.y, second.aabb.min.y) - glm::min(first.aabb.max.y, second.aabb.max.y)
		};

		auto normVel = glm::abs(vFirst) + glm::abs(vSecond);
		auto timeToCollision = glm::vec2 {
			(normVel.x != 0) ? distance.x / normVel.x : constants::finfinity,
			(normVel.y != 0) ? distance.y / normVel.y : constants::finfinity
		};

		setCollisionFlags(timeToCollision, vFirst, first.body->m_collisions, second.body->m_collisions);

		if (glm::abs(timeToCollision.x) < glm::abs(timeToCollision.y)) {
			auto dir = (timeToCollision.x == constants::finfinity) ? 0.0f : timeToCollision.x; // Vertical collision
			
			vFirst.x *= dir;
			vSecond.x *= dir;
		} else {
			auto dir = (timeToCollision.y == constants::finfinity) ? 0.0f : timeToCollision.y; // Horizontal collision

			vFirst.y *= dir;
			vSecond.y *= dir;
		}

		first.body->m_collidedLayers |= layers;
		second.body->m_collidedLayers |= layers;
	}
}

void PhysicsSystem::handleStaticCollision(KinematicEntity& kinematicEntity, StaticEntity& staticEntity, std::uint64_t layers) const {
	auto& vel = kinematicEntity.body->velocity;
	auto kinematicAABB = BoundingBox { kinematicEntity.aabb.min + vel, kinematicEntity.aabb.max + vel };

	if (kinematicAABB.max.x > staticEntity.aabb.min.x && 
		kinematicAABB.min.x < staticEntity.aabb.max.x && 
		kinematicAABB.max.y > staticEntity.aabb.min.y && 
		kinematicAABB.min.y < staticEntity.aabb.max.y
	) {
		auto distance = glm::vec2 {
			glm::max(kinematicEntity.aabb.min.x, staticEntity.aabb.min.x) - glm::min(kinematicEntity.aabb.max.x, staticEntity.aabb.max.x),
			glm::max(kinematicEntity.aabb.min.y, staticEntity.aabb.min.y) - glm::min(kinematicEntity.aabb.max.y, staticEntity.aabb.max.y)
		};

		auto absVel = glm::abs(vel);
		auto timeToCollision = glm::vec2 {
			(absVel.x != 0) ? distance.x / absVel.x : constants::finfinity,
			(absVel.y != 0) ? distance.y / absVel.y : constants::finfinity
		};

		setCollisionFlags(timeToCollision, vel, kinematicEntity.body->m_collisions, staticEntity.body->m_collisions);

		if (glm::abs(timeToCollision.x) < glm::abs(timeToCollision.y))
			vel.x *= ((timeToCollision.x == constants::finfinity) ? 0.0f : timeToCollision.x); // Vertical collision
		else
			vel.y *= ((timeToCollision.y == constants::finfinity) ? 0.0f : timeToCollision.y); // Horizontal collision

		std::printf("%f, %f\n", distance.x, distance.y);

		kinematicEntity.body->m_collidedLayers |= layers;
		staticEntity.body->m_collidedLayers |= layers;
	}
}

void PhysicsSystem::handleAreaCollision(KinematicEntity& kinematicEntity, AreaEntity& areaEntity, std::uint64_t layers) const {
	const auto& vel = kinematicEntity.body->velocity;
	auto kinematicAABB = BoundingBox { kinematicEntity.aabb.min + vel, kinematicEntity.aabb.max + vel };

	if (kinematicAABB.max.x > areaEntity.aabb.min.x && 
		kinematicAABB.min.x < areaEntity.aabb.max.x && 
		kinematicAABB.max.y > areaEntity.aabb.min.y && 
		kinematicAABB.min.y < areaEntity.aabb.max.y
	) {
		auto distance = glm::vec2 {
			glm::max(kinematicEntity.aabb.min.x, areaEntity.aabb.min.x) - glm::min(kinematicEntity.aabb.max.x, areaEntity.aabb.max.x),
			glm::max(kinematicEntity.aabb.min.y, areaEntity.aabb.min.y) - glm::min(kinematicEntity.aabb.max.y, areaEntity.aabb.max.y)
		};

		auto absVel = glm::abs(vel);
		auto timeToCollision = glm::vec2 {
			(absVel.x != 0) ? distance.x / absVel.x : constants::finfinity,
			(absVel.y != 0) ? distance.y / absVel.y : constants::finfinity
		};

		setCollisionFlags(timeToCollision, vel, kinematicEntity.body->m_collisions, areaEntity.body->m_collisions);

		kinematicEntity.body->m_collidedLayers |= layers;
		areaEntity.body->m_collidedLayers |= layers;
	}
}

void PhysicsSystem::setCollisionFlags(const glm::vec2& timeToCol, const glm::vec2& firstVel, Collision& first, Collision& second) const noexcept {
	if (timeToCol.x < timeToCol.y) {
		if (firstVel.x > 0) {
			first |= Collision::right;
			second |= Collision::left;
		} else {
			first |= Collision::left;
			second |= Collision::right;
		}
	} else {
		if (firstVel.y > 0) {
			first |= Collision::down;
			second |= Collision::up;
		} else {
			first |= Collision::up;
			second |= Collision::down;
		}
	}
}

} // namespace detail

} // namespace esengine
