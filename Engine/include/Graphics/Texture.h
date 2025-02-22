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

#include <Core/Common.h>
#include <Core/RAIIContainers.h>
#include <Core/FileSystem.h>

#include <Detail/RenderSystem.h>

#include <LSD/String.h>

#include <stb_image.h>

#include <SDL3/SDL.h>

#include <glm/glm.hpp>

namespace esengine {

// Utility enums, maps some SDL enump to enum classes

enum class BlendMode {
	none = SDL_BLENDMODE_NONE,
	blend = SDL_BLENDMODE_BLEND,
	blendPMul = SDL_BLENDMODE_BLEND_PREMULTIPLIED,
	add = SDL_BLENDMODE_ADD,
	addPMul = SDL_BLENDMODE_ADD_PREMULTIPLIED,
	mod = SDL_BLENDMODE_MOD,
	mul = SDL_BLENDMODE_MUL,

	invalid = SDL_BLENDMODE_INVALID
};


namespace detail {

// Texture base class

class BasicTexture {
public:
	BasicTexture(const glm::ivec3& dim = { }, sdl::Texture&& tx = { }) :
		m_texture(std::move(tx)), m_dimension(dim) { }

	void setAlphaMod(float a);
	void setColorMod(es_color_t col);
	void setBlendMode(BlendMode mode);

	void enableFiltering();
	void disableFiltering();

	[[nodiscard]] float alphaMod() const;
	[[nodiscard]] es_color_t colorMod() const;
	[[nodiscard]] BlendMode blendMode() const;

	[[nodiscard]] bool filtered() const;

	[[nodiscard]] SDL_Texture* texture() const noexcept {
		return m_texture;
	}
	[[nodiscard]] const glm::ivec3& dimension() const noexcept {
		return m_dimension;
	}

protected:
	sdl::Texture m_texture;
	glm::ivec3 m_dimension; // z represents the channels
};

} // namespace detail


class TextureData {
public:
	TextureData(lsd::StringView path);
	~TextureData();

	[[nodiscard]] stbi_uc* begin() noexcept {
		return m_data;
	}
	[[nodiscard]] const stbi_uc* end() noexcept {
		return m_data + m_dimension.x * m_dimension.y * m_dimension.z;
	}
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

	friend class Texture;
};

// Image texture
class Texture : public detail::BasicTexture {
public:
	Texture() = default;
	Texture(const TextureData& data);
	Texture(lsd::StringView path);

	[[nodiscard]] const lsd::String& path() const noexcept {
		return m_path;
	}

private:
	lsd::String m_path;
};

// Streaming texture
class StreamingTexture : public detail::BasicTexture {
public:
	StreamingTexture() = default;
	StreamingTexture(glm::ivec2 dimension);

	void resize(glm::ivec2 dimension);

	void lock(SDL_Surface*& surface, const SDL_Rect* location = nullptr);
	void unlock();
};

} // namespace esengine
