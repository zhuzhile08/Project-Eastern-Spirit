/************************
 * @file Tilemap.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Tilemap importer and renderer
 * 
 * @date 2024-11-27
 * @copyright Copyright (c) 2024
 ************************/

#pragma once

#include <LSD/Vector.h>
#include <LSD/StringView.h>

#include <glm/glm.hpp>

namespace esengine {

class Tilemap {
public:
	Tilemap(lsd::StringView path);

private:
	lsd::Vector<lsd::Vector<std::size_t>> m_map;

	glm::uvec2 m_size;
};

} // namespace esengine
