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
	void update(esengine::KinematicBody& kinematicBody, esengine::SpriteAnimator& animator) const;

private:

};
