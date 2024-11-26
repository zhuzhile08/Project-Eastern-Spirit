#include <Controllers/Player.h>

#include <Common/Math.h>
#include <InputSystem.h>

void PlayerController::update(float deltaTime, esengine::KinematicBody& kinematicBody, esengine::SpriteAnimator& animator) {
	auto& vel = kinematicBody.velocity;

	auto inputVec = getInputVec();

	updateAnimations(inputVec, animator);

	if (inputVec.x != 0 && inputVec.y != 0) inputVec *= 0.7071068f;
	vel = inputVec * m_speed * deltaTime;

	m_prevInputVec = inputVec;
}

glm::vec2 PlayerController::getInputVec() const {
	glm::vec2 res { };

	if (esengine::globals::inputSystem->keyboard(esengine::KeyType::w).held) res.y = -1;
	if (esengine::globals::inputSystem->keyboard(esengine::KeyType::s).held) res.y = 1;
	if (esengine::globals::inputSystem->keyboard(esengine::KeyType::a).held) res.x = -1;
	if (esengine::globals::inputSystem->keyboard(esengine::KeyType::d).held) res.x = 1;

	return res;
}

void PlayerController::updateAnimations(const glm::vec2& inputVec, esengine::SpriteAnimator& animator) const {
	if (inputVec.y > esengine::fepsilon) animator.play("WalkDown");
	else if (inputVec.y < -esengine::fepsilon) animator.play("WalkUp");
	else if (inputVec.x > esengine::fepsilon) animator.play("WalkRight");
	else if (inputVec.x < -esengine::fepsilon) animator.play("WalkLeft");
	else {
		if (m_prevInputVec.y > esengine::fepsilon) animator.play("IdleDown");
		else if (m_prevInputVec.y < -esengine::fepsilon) animator.play("IdleUp");
		else if (m_prevInputVec.x > esengine::fepsilon) animator.play("IdleRight");
		else if (m_prevInputVec.x < -esengine::fepsilon) animator.play("IdleLeft");
	}
}
