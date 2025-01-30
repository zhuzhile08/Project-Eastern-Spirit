#include <Components/Colliders.h>

namespace esengine {

namespace detail {

BoundingBox BasicCollider::aabb(const etcs::Transform& transform) const noexcept {
	auto pos = glm::vec2(transform.translation()) + offset;

	return { pos, pos + collider };
}

void BasicCollider::clearCollisions() noexcept {
	m_collisions = Collision::none;
	m_collidedLayers = 0;
}

#ifndef NDEBUG
void BasicCollider::debugDrawCall(detail::RenderSystem::CallData& data,
	const etcs::Entity& entity,
	const etcs::Transform& transform,
	const glm::mat4& camTransform,
	const Camera& camera) {
	
}
#endif

} // namespace detail

void KinematicBody::move(etcs::Transform& transform) {
	transform.translation().x += velocity.x;
	transform.translation().y += velocity.y;

	velocity = glm::vec2(0.0f);
}

} // namespace esengine
