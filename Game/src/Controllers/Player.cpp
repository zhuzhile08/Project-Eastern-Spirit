#include <Controllers/Player.h>

#include <InputSystem.h>

void PlayerController::update(esengine::KinematicBody& kinematicBody, esengine::SpriteAnimator& animator) const {
	if (esengine::globals::inputSystem->keyboard(esengine::KeyType::w).held) {
		kinematicBody.velocity.y -= 1;
	}
	if (esengine::globals::inputSystem->keyboard(esengine::KeyType::s).held) {
		kinematicBody.velocity.y += 1;
	}
	if (esengine::globals::inputSystem->keyboard(esengine::KeyType::a).held) {
		kinematicBody.velocity.x -= 1;
	}
	if (esengine::globals::inputSystem->keyboard(esengine::KeyType::d).held) {
		kinematicBody.velocity.x += 1;
	}
}
