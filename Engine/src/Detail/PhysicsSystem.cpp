#include <Detail/PhysicsSystem.h>

#include <Core/Math.h>

#include <ETCS/ETCS.h>

namespace esengine {

namespace globals {

detail::PhysicsSystem* physicsSystem = nullptr;

}

namespace detail {

void PhysicsSystem::update() {
	for (auto [transform, kinematicBody] : etcs::world().query<etcs::Transform, KinematicBody>())
		m_kinematics.emplaceBack(&transform, &kinematicBody, kinematicBody.aabb(transform));
	
	for (auto [transform, staticBody] : etcs::world().query<etcs::Transform, StaticBody>())
		m_statics.emplaceBack(&transform, &staticBody, staticBody.aabb(transform));

	for (std::size_t i = 0; i < m_kinematics.size(); i++) {
		auto kinematicEntity = m_kinematics[i];

		for (auto j = i + 1; j < m_kinematics.size(); j++) {
			auto& second = m_kinematics[j];

			Manifest manifest { kinematicEntity.body->layers() & second.body->layers() };

			if (manifest.layers != 0 && aabbCollided(manifest, kinematicEntity.aabb, kinematicEntity.body->velocity, second.aabb, second.body->velocity))
				handleKinematicCollision(manifest, kinematicEntity, second);
		}

		for (auto& staticEntity : m_statics) {
			Manifest manifest { kinematicEntity.body->layers() & staticEntity.body->layers() };

			if (manifest.layers != 0 && aabbCollided(manifest, kinematicEntity.aabb, kinematicEntity.body->velocity, staticEntity.aabb, { 0.0f, 0.0f } ))
				handleStaticCollision(manifest, kinematicEntity, staticEntity);
		}
	}

	// apply velocity to objects
	for (auto& [transform, kinematicBody, _] : m_kinematics) {
		kinematicBody->move(*transform);
	}

	// clear all data from the frame
	m_kinematics.clear();
	m_statics.clear();
}

bool PhysicsSystem::aabbCollided(
	Manifest& manifest, 
	BoundingBox& first, 
	const glm::vec2& vFirst, 
	BoundingBox& second,
	const glm::vec2& vSecond
) const {
	auto absVel = glm::abs(vFirst) + glm::abs(vSecond);

	auto distanceTranslated = glm::vec2(
		glm::max(first.min.x + vFirst.x, second.min.x + vSecond.x) - glm::min(first.max.x + vFirst.x, second.max.x + vSecond.x),
		glm::max(first.min.y + vFirst.y, second.min.y + vSecond.y) - glm::min(first.max.y + vFirst.y, second.max.y + vSecond.y)
	);

	if (distanceTranslated.x < absVel.x && distanceTranslated.y < absVel.y) {
		auto distance = glm::vec2(
			glm::max(first.min.x, second.min.x) - glm::min(first.max.x, second.max.x),
			glm::max(first.min.y, second.min.y) - glm::min(first.max.y, second.max.y)
		);

		auto timeToCollision = glm::vec2(
			(absVel.x != 0) ? distance.x / absVel.x : constants::finfinity,
			(absVel.y != 0) ? distance.y / absVel.y : constants::finfinity
		);

		if (glm::abs(timeToCollision.x) < glm::abs(timeToCollision.y)) 
			manifest.penetrationDirection.x = timeToCollision.x; // vertical collision
		else
			manifest.penetrationDirection.y = timeToCollision.y; // horizontal collision

		return true;
	}

	return false;
}

void PhysicsSystem::handleKinematicCollision(const Manifest& manifest, KinematicEntity& first, KinematicEntity& second) const {
	first.body->velocity *= manifest.penetrationDirection;
	second.body->velocity *= manifest.penetrationDirection;

	first.body->m_collided = true;
	second.body->m_collided = true;

	first.body->m_collidedLayers |= manifest.layers;
	second.body->m_collidedLayers |= manifest.layers;
}

void PhysicsSystem::handleStaticCollision(const Manifest& manifest, KinematicEntity& kinematicEntity, StaticEntity& staticEntity) const {
	kinematicEntity.body->velocity *= manifest.penetrationDirection;

	kinematicEntity.body->m_collided = true;
	staticEntity.body->m_collided = true;

	kinematicEntity.body->m_collidedLayers |= manifest.layers;
	staticEntity.body->m_collidedLayers |= manifest.layers;
}

} // namespace detail

} // namespace esengine
