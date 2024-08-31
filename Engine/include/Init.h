/*************************
 * @file Application.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Application class
 * 
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 *************************/

#pragma once

#include <SDL3/SDL.h>

#include <LSD/StringView.h>
#include <glm/glm.hpp>

namespace esengine {

struct InitInfo {
	char** argv = { };

	lsd::StringView name = { };
	glm::ivec2 dim = { 1280, 720 };
	int flags = { };
};

void init(InitInfo info);
void quit();

} // namespace esengine
