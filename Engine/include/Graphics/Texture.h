/*************************
 * @file Texture.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Texture wrapper class
 * 
 * @date 2024-09-01
 * 
 * @copyright Copyright (c) 2024
 *************************/

#pragma once

#include <SDL3/SDL.h>

#include <Common/RAIIContainers.h>

namespace esengine {

class Texture {
public:
	Texture(lsd::StringView path);

	[[nodiscard]] SDL_Texture* texture() const noexcept {
		return m_texture.get();
	}

private:
	sdl::Texture m_texture;
};

} // namespace esengine
