/************************
 * @file SpriteAnimator.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Animaton system for storing animations and applying them to sprites
 * 
 * @date 2024-11-19
 * @copyright Copyright (c) 2024
 ************************/

#pragma once

#include <Common/Common.h>
#include <Components/Sprite.h>

#include <LSD/Vector.h>
#include <LSD/String.h>
#include <LSD/StringView.h>
#include <LSD/UnorderedSparseMap.h>

#include <glm/glm.hpp>

#include <chrono>

namespace esengine {

class SpriteAnimator {
public:
	struct Frame {
		std::size_t index;
		std::chrono::milliseconds time;
	};

	struct Animation {
	public:
		lsd::String name;
		lsd::Vector<Frame> frames;

		bool repeated = false;
	};
	
	struct AnimPlayData {
	public:
		[[nodiscard]] bool playing() const noexcept {
			return name.empty() || speed > 0.f;
		}

		lsd::StringView name { };
		std::size_t frameIndex { };
		std::uint64_t frameTime { };
		float speed = 1.f;
	};

private:
	CUSTOM_HASHER(AnimHash, const Animation&, const lsd::String&, lsd::Hash<lsd::String>(), .name)
	CUSTOM_EQUAL(AnimEqual, const Animation&, const lsd::String&, .name)

public:
	SpriteAnimator(const etcs::ComponentView<Sprite>& sprite) : m_sprite(sprite) { }
	SpriteAnimator(const etcs::ComponentView<Sprite>& sprite, Animation&& autoPlay);

	void addAnimation(Animation&& anim) {
		m_animations.emplace(anim);
	}

	void play(lsd::StringView animationName, float speed = 1.f);
	void pause() {
		if (m_current.playing()) m_current.speed = 0.f;
	}
	void stop() {
		m_current = AnimPlayData();
	}

	void setFrame(std::size_t frame, std::uint64_t startingTime = 0.f);

	void queue(lsd::StringView animationName, float speed = 1.f);
	void clearQueue() {
		m_queue.clear();
	}

	[[nodiscard]] const AnimPlayData& current() const noexcept {
		return m_current;
	}

	[[nodiscard]] lsd::StringView autoPlay() const noexcept {
		return m_autoPlay;
	}
	[[nodiscard]] etcs::ComponentView<Sprite> sprite() const noexcept {
		return m_sprite;
	}

private:
	lsd::UnorderedSparseSet<Animation, AnimHash, AnimEqual> m_animations;

	AnimPlayData m_current;
	lsd::Vector<AnimPlayData> m_queue;

	lsd::StringView m_autoPlay;

	etcs::ComponentView<Sprite> m_sprite;

	void update(std::uint64_t deltaTime);

	friend class Application;
};

} // namespace esengine
