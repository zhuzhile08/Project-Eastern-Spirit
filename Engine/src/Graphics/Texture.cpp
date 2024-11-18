#define STB_IMAGE_IMPLEMENTATION // Do not remove this, just leave this at the top of the file

#include <Common/Common.h>

#include <Graphics/Texture.h>
#include <Graphics/RenderSystem.h>

namespace esengine {

using namespace lsd::enum_operators;

// Raw texture data

TextureData::TextureData(lsd::StringView path) : m_path(path) {
	File file = globals::fileSystem->load(globals::fileSystem->assetsPath() / m_path.data(), OpenMode::binary | OpenMode::read, false);

	m_data = stbi_load_from_file(file.stream(), &m_dimension.x, &m_dimension.y, &m_dimension.z, STBI_rgb_alpha);
	if (m_data == nullptr) throw std::runtime_error("esengine::TextureData::TextureData(): Failed to load texture!");
}

TextureData::~TextureData() {
	stbi_image_free(m_data);

	m_data = nullptr;
}


// Image texture

Texture::Texture(lsd::StringView path, std::initializer_list<lsd::StringView> passNames) : m_path(path) {
	TextureData data(path);
	m_dimension = data.dimension();

	for (const auto& name : passNames) createForPass(name, data);
}

SDL_Texture* Texture::texture(lsd::StringView passName) noexcept {
	auto it = m_textures.find(passName);

	if (it != m_textures.end()) return it->second.get();
	else {
		TextureData data(m_path);

		return createForPass(passName, data);
	}
}

SDL_Texture* Texture::createForPass(lsd::StringView passName, const TextureData& data) {
	auto& tx = m_textures.emplace(passName, sdl::Texture(
		globals::renderSystem->pass(passName).renderer,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STATIC,
		m_dimension.x,
		m_dimension.y)
	).first->second;

	if (!tx) throw std::runtime_error("esengine::Texture::createForPass(): Failed to create SDL image texture!");

	if (SDL_UpdateTexture(tx.get(), nullptr, data.data(), m_dimension.x * m_dimension.z) == false)
		throw std::runtime_error("esengine::Texture::createForPass(): Failed to update SDL image texture!");

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

void StreamingTexture::lock(SDL_Surface*& surface, const SDL_Rect* location, lsd::StringView passName) {
	if (!SDL_LockTextureToSurface(m_textures.at(passName).get(), location, &surface)) throw std::runtime_error("esengine::StreamingTexture::lock(): Failed to retrieve surface from streaming texture!");
}

void StreamingTexture::unlock(lsd::StringView passName) {
	SDL_UnlockTexture(m_textures.at(passName).get());
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
