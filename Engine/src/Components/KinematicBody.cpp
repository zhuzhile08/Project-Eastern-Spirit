#include <Components/KinematicBody.h>

namespace esengine {

void KinematicBody::move(etcs::Transform& transform) {
	transform.translation().x += velocity.x;
	transform.translation().y += velocity.y;

	velocity = glm::vec2 { };
}

BoundingBox KinematicBody::aabb(const etcs::Transform& transform) const noexcept {
	auto pos = glm::vec2(transform.translation()) + m_offset;

	return { pos, pos + m_collider };
}

/*
BoundingBox KinematicBody::bbb(const etcs::Transform& transform) const noexcept {
	auto pos = glm::vec2(transform.translation()) + m_offset;

	if (velocity.y > 0) {
		if (velocity.x > 0) return { // right-up
			pos + glm::vec2(0.0f, velocity.y),
			pos - glm::vec2(0.0f, m_collider.y) + glm::vec2(velocity.x, 0.0f)
		};
		else return { // left-up
			pos + velocity, 
			pos + glm::vec2(m_collider.x, 0.0f) - glm::vec2(0.0f, m_collider.y)
		};
	} else {
		if (velocity.x > 0) return { // right-down
			pos,
			pos + m_collider + velocity
		};
		else return { // left-down
			pos + glm::vec2(m_collider.x, 0.0f),
			pos + velocity + glm::vec2(0.0f, m_collider.y)
		};
	}
}
*/

} // namespace esengine
