/*************************
 * @file Texture.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Texture wrapper classes
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

#include <stb_image.h>

#include <SDL3/SDL.h>

#include <glm/glm.hpp>

namespace esengine {

class TextureData {
public:
	TextureData(lsd::StringView path);
	~TextureData();

	[[nodiscard]] const stbi_uc* data() const noexcept {
		return m_data;
	}
	[[nodiscard]] const glm::ivec3& dimension() const noexcept {
		return m_dimension;
	}
	[[nodiscard]] const lsd::String& path() const noexcept {
		return m_path;
	}

private:
	stbi_uc* m_data;
	glm::ivec3 m_dimension; // z represents the channels

	lsd::String m_path;
};

// Image texture
class Texture {
public:
	Texture(lsd::StringView path, std::initializer_list<lsd::StringView> passNames = { { } });

	[[nodiscard]] SDL_Texture* texture(lsd::StringView passName = { }) noexcept;
	[[nodiscard]] const SDL_Texture* texture(lsd::StringView passName = { }) const noexcept {
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

	SDL_Texture* createForPass(lsd::StringView passName, const TextureData& data);
};

// Streaming texture
class StreamingTexture {
public:
	StreamingTexture(glm::ivec2 dimension, std::initializer_list<lsd::StringView> passNames = { { } });

	void resize(glm::ivec2 dimension);
	void update(const void* data, SDL_Rect location, lsd::StringView passName = { });

	[[nodiscard]] SDL_Texture* texture(lsd::StringView passName = { }) noexcept;
	[[nodiscard]] const SDL_Texture* texture(lsd::StringView passName = { }) const noexcept {
		return m_textures.at(passName).get();
	}
	[[nodiscard]] const glm::ivec2& dimension() const noexcept {
		return m_dimension;
	}

private:
	lsd::UnorderedSparseMap<lsd::String, sdl::Texture> m_textures;
	glm::ivec2 m_dimension = { }; // z represents the channels

	SDL_Texture* createForPass(lsd::StringView passName);
};

} // namespace esengine
