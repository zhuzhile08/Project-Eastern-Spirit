/************************
 * @file MapBuilder.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Manager responsible for loading maps and levels
 * 
 * @date 2024-11-29
 * @copyright Copyright (c) 2024
 ************************/

#pragma once

#include <LSD/Vector.h>
#include <LSD/StringView.h>

#include <glm/glm.hpp>

namespace esengine {

class MapManager {
public:
	MapManager(lsd::StringView worldPath);

private:
	//lsd::Vector<lsd::Vector<>> m_screens;

	glm::uvec2 m_currentScreen;
	glm::uvec2 m_size;
};

} // namespace esengine
