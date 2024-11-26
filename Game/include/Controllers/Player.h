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

#include <Components/KinematicBody.h>
#include <Components/SpriteAnimator.h>

class PlayerController {
public:
	void update(float deltaTime, esengine::KinematicBody& kinematicBody, esengine::SpriteAnimator& animator);

private:
	float m_speed = 1;

	glm::vec2 m_prevInputVec = { };

	glm::vec2 getInputVec() const;
	void updateAnimations(const glm::vec2& inputVec, esengine::SpriteAnimator& animator) const;
};
