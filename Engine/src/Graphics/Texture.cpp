#define STB_IMAGE_IMPLEMENTATION // Do not remove this, just leave this at the top of the file

#include <Common/Common.h>

#include <Graphics/Texture.h>
#include <Graphics/RenderSystem.h>

#include <stb_image.h>

namespace esengine {

using namespace lsd::enum_operators;

Texture::Texture(lsd::StringView path, std::initializer_list<lsd::StringView> passNames) : m_path(path) {
	File f = globals::fileSystem->load(globals::fileSystem->assetsPath() / path.data(), OpenMode::binary | OpenMode::read, false);

	for (const auto& name : passNames) createForPassBackend(name, f);
}

void Texture::createForPass(lsd::StringView passName) {
	File f = globals::fileSystem->load(globals::fileSystem->assetsPath() / m_path.data(), OpenMode::binary | OpenMode::read, false);

	createForPassBackend(passName, f);
}

void Texture::createForPassBackend(lsd::StringView passName, const File& file) {
	auto pixels = stbi_load_from_file(file.stream(), &m_dimension.x, &m_dimension.y, &m_dimension.z, STBI_rgb_alpha);
	if (pixels == nullptr) throw std::runtime_error("Failed to load sprite sheet!");

	auto& tx = m_textures.emplace(passName, sdl::Texture(
		globals::renderSystem->pass(passName).renderer, 
		SDL_PIXELFORMAT_RGBA32, 
		SDL_TEXTUREACCESS_STATIC, 
		m_dimension.x, 
		m_dimension.y)
	).first->second;

	if (!tx) throw std::runtime_error("Failed to create SDL texture!");

	if (SDL_Rect dim { 0, 0, m_dimension.x, m_dimension.y }; SDL_UpdateTexture(tx.get(), &dim, pixels, m_dimension.x * m_dimension.z) == SDL_FALSE)
		throw std::runtime_error("Failed to update SDL texutre!");

	stbi_image_free(pixels);
}

} // namespace esengine
