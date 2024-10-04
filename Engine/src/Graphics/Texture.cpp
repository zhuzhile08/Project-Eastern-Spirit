#define STB_IMAGE_IMPLEMENTATION // Do not remove this, just leave this at the top of the file

#include <Common/Common.h>

#include <Graphics/Texture.h>
#include <Graphics/RenderSystem.h>

#include <stb_image.h>

namespace esengine {

using namespace lsd::enum_operators;

// Image texture

Texture::Texture(lsd::StringView path, std::initializer_list<lsd::StringView> passNames) : m_path(path) {
	File f = globals::fileSystem->load(globals::fileSystem->assetsPath() / path.data(), OpenMode::binary | OpenMode::read, false);

	for (const auto& name : passNames) createForPass(name, f);
}

SDL_Texture* Texture::texture(lsd::StringView passName) noexcept {
	auto it = m_textures.find(passName);

	if (it != m_textures.end()) return it->second.get();
	else {
		File f = globals::fileSystem->load(globals::fileSystem->assetsPath() / m_path.data(), OpenMode::binary | OpenMode::read, false);

		return createForPass(passName, f);
	}
}

SDL_Texture* Texture::createForPass(lsd::StringView passName, const File& file) {
	auto pixels = stbi_load_from_file(file.stream(), &m_dimension.x, &m_dimension.y, &m_dimension.z, STBI_rgb_alpha);
	if (pixels == nullptr) throw std::runtime_error("Failed to load sprite sheet!");

	auto& tx = m_textures.emplace(passName, sdl::Texture(
		globals::renderSystem->pass(passName).renderer, 
		SDL_PIXELFORMAT_RGBA32, 
		SDL_TEXTUREACCESS_STATIC, 
		m_dimension.x, 
		m_dimension.y)
	).first->second;

	if (!tx) throw std::runtime_error("Failed to create SDL image texture!");

	if (SDL_UpdateTexture(tx.get(), nullptr, pixels, m_dimension.x * m_dimension.z) == false)
		throw std::runtime_error("Failed to update SDL image texture!");

	stbi_image_free(pixels);

	return tx.get();
}


// Streaming texture

StreamingTexture::StreamingTexture(glm::ivec2 dimension, std::initializer_list<lsd::StringView> passNames) : m_dimension(dimension) {
	for (const auto& name : passNames) createForPass(name);
}

void StreamingTexture::resize(glm::ivec2 dimension) {
	m_dimension = dimension;

	for (auto& [name, texture] : m_textures) {
		texture = sdl::Texture(
			globals::renderSystem->pass(name).renderer, 
			SDL_PIXELFORMAT_RGBA32, 
			SDL_TEXTUREACCESS_STREAMING, 
			m_dimension.x, 
			m_dimension.y
		);

		if (!texture) throw std::runtime_error("Failed to create SDL texture for streaming!");
	}
}

void StreamingTexture::update(const void* data, SDL_Rect location, lsd::StringView passName) {
	if (SDL_UpdateTexture(m_textures.at(passName).get(), &location, data, m_dimension.x * 4) == false)
		throw std::runtime_error("Failed to update SDL image texture!");
}

SDL_Texture* StreamingTexture::texture(lsd::StringView passName) noexcept {
	auto it = m_textures.find(passName);

	if (it != m_textures.end()) return it->second.get();
	else return createForPass(passName);
}

SDL_Texture* StreamingTexture::createForPass(lsd::StringView passName) {
	auto& tx = m_textures.emplace(passName, sdl::Texture(
		globals::renderSystem->pass(passName).renderer, 
		SDL_PIXELFORMAT_RGBA32, 
		SDL_TEXTUREACCESS_STREAMING, 
		m_dimension.x, 
		m_dimension.y)
	).first->second;

	if (!tx) throw std::runtime_error("Failed to create SDL texture for streaming!");

	return tx.get();
}

} // namespace esengine
