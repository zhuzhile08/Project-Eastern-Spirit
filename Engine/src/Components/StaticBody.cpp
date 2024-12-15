#include <Components/StaticBody.h>

namespace esengine {

BoundingBox StaticBody::aabb(const etcs::Transform& transform) const noexcept {
	auto pos = glm::vec2(transform.translation()) + m_offset;

	return { pos, pos + m_collider };
}

} // namespace esengine
