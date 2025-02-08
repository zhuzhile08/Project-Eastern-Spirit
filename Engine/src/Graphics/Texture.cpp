#define STB_IMAGE_IMPLEMENTATION // Do not remove this, just leave this at the top of the file

#include <Core/Common.h>

#include <Graphics/Texture.h>
#include <Detail/RenderSystem.h>

using namespace lsd::enum_operators;

namespace esengine {

namespace detail {

void BasicTexture::setAlphaMod(float a) {
	SDL_SetTextureAlphaModFloat(m_texture, a);
}

void BasicTexture::setColorMod(es_color_t col) {
	SDL_SetTextureColorModFloat(m_texture, col.x, col.y, col.z);
}

void BasicTexture::setBlendMode(BlendMode mode) {
	SDL_SetTextureBlendMode(m_texture, static_cast<SDL_BlendMode>(mode));
}

void BasicTexture::enableFiltering() {
	SDL_SetTextureScaleMode(m_texture, SDL_SCALEMODE_LINEAR);
}

void BasicTexture::disableFiltering() {
	SDL_SetTextureScaleMode(m_texture, SDL_SCALEMODE_NEAREST);
}

float BasicTexture::alphaMod() const {
	float res;
	SDL_GetTextureAlphaModFloat(m_texture, &res);
	return res;
}

es_color_t BasicTexture::colorMod() const {
	es_color_t res;
	SDL_GetTextureColorModFloat(m_texture, &res.x, &res.y, &res.z);
	return res;
}

BlendMode BasicTexture::blendMode() const {
	SDL_BlendMode res;
	SDL_GetTextureBlendMode(m_texture, &res);
	return static_cast<BlendMode>(res);
}

bool BasicTexture::filtered() const {
	SDL_ScaleMode res;
	SDL_GetTextureScaleMode(m_texture, &res);
	return res == SDL_SCALEMODE_LINEAR;
}

} // namespace detail


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

Texture::Texture(const TextureData& data) : 
	detail::BasicTexture(
		data.m_dimension,
		sdl::Texture (
			globals::renderSystem->renderer(),
			SDL_PIXELFORMAT_RGBA32,
			SDL_TEXTUREACCESS_STATIC,
			data.m_dimension.x,
			data.m_dimension.y
		)
	),
	m_path(data.m_path)
{
	if (!m_texture) throw std::runtime_error("esengine::Texture::Texture(): Failed to create SDL image texture!");

	if (!SDL_UpdateTexture(m_texture, nullptr, data.data(), m_dimension.x * m_dimension.z))
		throw std::runtime_error("esengine::Texture::Texture(): Failed to update SDL image texture!");
}

Texture::Texture(lsd::StringView path) {
	auto data = TextureData(path);

	m_dimension = data.m_dimension;

	m_texture = sdl::Texture(
		globals::renderSystem->renderer(),
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STATIC,
		m_dimension.x,
		m_dimension.y
	);

	if (!m_texture) throw std::runtime_error("esengine::Texture::Texture(): Failed to create SDL image texture!");

	if (!SDL_UpdateTexture(m_texture, nullptr, data.data(), m_dimension.x * m_dimension.z))
		throw std::runtime_error("esengine::Texture::Texture(): Failed to update SDL image texture!");

	m_path = std::move(data.m_path);
}


// Streaming texture

StreamingTexture::StreamingTexture(glm::ivec2 dimension) :
	detail::BasicTexture(
		glm::ivec3(dimension, 4),
		sdl::Texture(
			globals::renderSystem->renderer(), 
			SDL_PIXELFORMAT_RGBA32, 
			SDL_TEXTUREACCESS_STREAMING, 
			dimension.x, 
			dimension.y
		)
	) {
	if (!m_texture) throw std::runtime_error("esengine::StreamingTexture::StreamingTexture(): Failed to create SDL texture for streaming!");
}

void StreamingTexture::resize(glm::ivec2 dimension) {
	m_dimension = glm::ivec3(dimension, 4);

	m_texture = sdl::Texture(
		globals::renderSystem->renderer(), 
		SDL_PIXELFORMAT_RGBA32, 
		SDL_TEXTUREACCESS_STREAMING, 
		m_dimension.x, 
		m_dimension.y
	);

	if (!m_texture) throw std::runtime_error("Failed to resize SDL texture for streaming!");
}

void StreamingTexture::lock(SDL_Surface*& surface, const SDL_Rect* location) {
	if (!SDL_LockTextureToSurface(m_texture, location, &surface)) throw std::runtime_error("esengine::StreamingTexture::lock(): Failed to retrieve surface from streaming texture!");
}

void StreamingTexture::unlock() {
	SDL_UnlockTexture(m_texture);
}

} // namespace esengine
