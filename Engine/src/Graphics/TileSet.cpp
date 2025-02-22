#include <Graphics/TileSet.h>

#include <Core/FileSystem.h>

#include <LSD/String.h>
#include <LSD/JSON.h>

#include <filesystem>

namespace esengine {

TileSet::TileSet(lsd::StringView path) {
	File file = globals::fileSystem->load(path.data(), OpenMode::read, false);

	lsd::String data;
	data.resize(file.size());
	file.read(data.data(), data.size());
	lsd::Json json = lsd::Json::parse(data);

	const auto& tileSize = json.at("dim").array();
	m_tileSize = { tileSize.at(0).signedInt(), tileSize.at(1).unsignedInt() };
	m_texture = Texture(json.at("img").string());

	glm::ivec2 tileCount = {
		m_texture.dimension().x / m_tileSize.x,
		m_texture.dimension().y / m_tileSize.y
	};

	m_tiles.reserve(tileCount.x * tileCount.y);
	for (int y = 0; y < tileCount.y; y++) {
		for (int x = 0; x < tileCount.x; x++) {
			m_tiles.emplaceBack(SDL_FRect {
				static_cast<float>(x * m_tileSize.x),
				static_cast<float>(y * m_tileSize.y),
				static_cast<float>(m_tileSize.x),
				static_cast<float>(m_tileSize.y)
			});
		}
	}

	const auto& animations = json["anim"].array();
	for (const auto& animation : animations) {
		auto& animator = m_animators.emplaceBack(TileAnimator {
			.frameTime = static_cast<ms_time_t>(animation.at("time").unsignedInt()),
			.tileIndex = static_cast<ms_time_t>(animation.at("idx").unsignedInt())
		});

		const auto& frames = animation.at("frame");
		for (const auto& frame : frames) {
			animator.frames.pushBack(SDL_FRect {
				static_cast<float>(frame[0].unsignedInt()),
				static_cast<float>(frame[1].unsignedInt()),
				static_cast<float>(frame[2].unsignedInt()),
				static_cast<float>(frame[3].unsignedInt())
			});
		}
	}
}

void TileSet::update(ms_time_t deltaTime) {
	for (auto& animator : m_animators) {
		animator.currentTime += deltaTime;

		if (animator.currentTime >= animator.frameTime) {
			animator.currentTime %= animator.frameTime;

			++animator.frameIndex;

			if (animator.frameIndex >= animator.frames.size())
				animator.frameIndex = 0;

			m_tiles[animator.tileIndex].rect = animator.frames[animator.frameIndex];
		}
	}
}

const SDL_FRect& TileSet::at(std::size_t c) const {
	return m_tiles.at(c).rect;
}

} // namespace esengine
