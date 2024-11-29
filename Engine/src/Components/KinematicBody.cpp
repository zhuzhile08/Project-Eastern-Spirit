#include <Components/KinematicBody.h>

namespace esengine {

bool KinematicBody::move(etcs::Transform& transform) {
	transform.translation().x += velocity.x;
	transform.translation().y += velocity.y;

	velocity = glm::vec2 { };

	return true;
}

} // namespace esengine
