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

#include <Common/RAIIContainers.h>
#include <Common/FileSystem.h>

#include <LSD/String.h>
#include <LSD/UnorderedSparseMap.h>

#include <SDL3/SDL.h>

#include <glm/glm.hpp>

namespace esengine {

class Texture {
public:
	Texture(lsd::StringView path, std::initializer_list<lsd::StringView> passNames = { });

	void createForPass(lsd::StringView passName);

	[[nodiscard]] SDL_Texture* texture(lsd::StringView passName = "") const noexcept {
		return m_textures.at(passName).get();
	}
	[[nodiscard]] const glm::ivec3& dimension() const noexcept {
		return m_dimension;
	}
	[[nodiscard]] const lsd::String& path() const noexcept {
		return m_path;
	}

private:
	lsd::UnorderedSparseMap<lsd::String, sdl::Texture> m_textures;
	glm::ivec3 m_dimension = { }; // z represents the channels

	lsd::String m_path;

	void createForPassBackend(lsd::StringView passName, const File& file);
};

} // namespace esengine
