#define STB_IMAGE_IMPLEMENTATION // Do not remove this, just leave this at the top of the file

#include <Common/Common.h>

#include <Graphics/Texture.h>
#include <Graphics/RenderSystem.h>

#include <stb_image.h>

namespace esengine {

Texture::Texture(lsd::StringView path) {
	auto pixels = stbi_load( path.data(), &m_dimension.x, &m_dimension.y, &m_dimension.z, STBI_rgb_alpha);
	if (pixels == nullptr) {
		std::printf("Failed to load sprite sheet!");
	}

	if (m_texture = sdl::Texture(globals::renderSystem->, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, m_dimension.x, m_dimension.y); m_texture)
		throw std::runtime_error("Failed to create SDL texture!");

	if (SDL_Rect dim{ 0, 0, m_dimension.x, m_dimension.y }; SDL_UpdateTexture(m_texture.get(), &dim, pixels, m_dimension.x * m_dimension.z) == SDL_FALSE)
		throw std::runtime_error("Failed to update SDL texutre!");

	stbi_image_free(pixels);
}

} // namespace esengine
