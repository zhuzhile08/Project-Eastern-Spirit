/************************
 * @file KinematicBody.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief 
 * 
 * @date 2024-11-26
 * @copyright Copyright (c) 2024
 ************************/

#pragma once

#include <glm/glm.hpp>

#include <ETCS/Components/Transform.h>

namespace esengine {

class KinematicBody {
public:
	// bool move(etcs::Transform& transform, const Collider& collider);
	bool move(etcs::Transform& transform);

	std::size_t collisionLayer = 0;
	glm::vec2 velocity;
private:

};

} // namespace esengine
