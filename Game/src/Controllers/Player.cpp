#include <Controllers/Player.h>

#include <Core/Math.h>
#include <Detail/InputSystem.h>

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

void PlayerController::updateAnimations(const glm::vec2& inputVec, esengine::SpriteAnimator& animator) {
	if (m_direction == Direction::left || m_direction == Direction::right) {
		if (inputVec.x == 1) {
			m_direction = Direction::right;
			animator.play("WalkRight");
		} else if (inputVec.x == -1) {
			m_direction = Direction::left;
			animator.play("WalkLeft");
		} else if (inputVec.y == 1) {
			m_direction = Direction::down;
			animator.play("WalkDown");
		} else if (inputVec.y == -1) {
			m_direction = Direction::up;
			animator.play("WalkUp");
		} else if (m_direction == Direction::left) {
			animator.play("IdleLeft");
		} else animator.play("IdleRight");
	} else if (m_direction == Direction::up || m_direction == Direction::down) {
		if (inputVec.y == 1) {
			m_direction = Direction::down;
			animator.play("WalkDown");
		} else if (inputVec.y == -1) {
			m_direction = Direction::up;
			animator.play("WalkUp");
		} else if (inputVec.x == 1) {
			m_direction = Direction::right;
			animator.play("WalkRight");
		} else if (inputVec.x == -1) {
			m_direction = Direction::left;
			animator.play("WalkLeft");
		} else if (m_direction == Direction::up) {
			animator.play("IdleUp");
		} else animator.play("IdleDown");
	}
}
