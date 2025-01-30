/*************************
 * @file Player.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Player class
 * 
 * @date 2024-11-25
 * @copyright Copyright (c) 2024
 *************************/

#pragma once

#include <Components/Colliders.h>
#include <Components/SpriteAnimator.h>

class PlayerController {
private:
	enum class Direction {
		left,
		right,
		up,
		down
	};

public:
	void update(float deltaTime, esengine::KinematicBody& kinematicBody, esengine::SpriteAnimator& animator);

private:
	float m_speed = 1;
	glm::vec2 m_prevInputVec = { };

	Direction m_direction;

	glm::vec2 getInputVec() const;
	void updateAnimations(const glm::vec2& inputVec, esengine::SpriteAnimator& animator);
};
