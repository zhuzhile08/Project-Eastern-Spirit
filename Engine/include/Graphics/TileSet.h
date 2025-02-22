/************************
 * @file TileSet.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Load tile set as a resuable graphics asset
 * 
 * @date 2025-02-11
 * @copyright Copyright (c) 2025
 ************************/

#pragma once

#include <Core/Common.h>
#include <Core/Animation.h>

#include <Graphics/Texture.h>

#include <LSD/Vector.h>
#include <LSD/StringView.h>

#include <glm/glm.hpp>

namespace esengine {

class TileSet {
private:
	struct Tile {
		SDL_FRect rect;
		std::size_t colliderIndex { };
		std::size_t colliderCount { };
	};

	struct Collider {
		glm::vec2 collider;
		glm::vec2 offset;
	};

	struct TileAnimator {
		ms_time_t frameTime = constants::defaultAnimationFrameTime;
		ms_time_t currentTime = 0;

		std::size_t frameIndex = 0;
		lsd::Vector<SDL_FRect> frames;

		std::size_t tileIndex { };
	};

public:
	TileSet(lsd::StringView path);

	void update(ms_time_t deltaTime);

	[[nodiscard]] const glm::ivec2& tileSize() const noexcept {
		return m_tileSize;
	}
	[[nodiscard]] Texture& texture() noexcept {
		return m_texture;
	}
	[[nodiscard]] const Texture& texture() const noexcept {
		return m_texture;
	}

	[[nodiscard]] const SDL_FRect& at(std::size_t c) const;

private:
	Texture m_texture;

	glm::ivec2 m_tileSize;
	lsd::Vector<Tile> m_tiles;
	lsd::Vector<TileAnimator> m_animators;
	lsd::Vector<Collider> m_colliders;
};

} // namespace esengine
