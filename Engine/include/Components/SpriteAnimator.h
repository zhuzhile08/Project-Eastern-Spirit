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
#include <Common/Animation.h>
#include <Components/Sprite.h>

#include <LSD/Vector.h>
#include <LSD/StringView.h>
#include <LSD/UnorderedSparseMap.h>

#include <glm/glm.hpp>

#include <chrono>

namespace esengine {

class SpriteAnimator {
public:
	ESENGINE_DEFAULT_CONSTRUCTORS(SpriteAnimator)

	void play(lsd::StringView animationName, float speed = 1.f);
	void pause() {
		if (m_current.playing()) m_current.speed = 0.f;
	}
	void stop();

	void setFrame(std::size_t frame, std::uint64_t startingTime = 0.f);

	void queue(lsd::StringView animationName, float speed = 1.f);
	void clearQueue() {
		m_queue.clear();
	}

	[[nodiscard]] const detail::AnimationPlayData& current() const noexcept {
		return m_current;
	}

	[[nodiscard]] lsd::StringView autoPlay() const noexcept {
		return m_autoPlay;
	}
	[[nodiscard]] etcs::ComponentView<Sprite> sprite() const noexcept {
		return m_sprite;
	}

private:
	lsd::UnorderedSparseSet<Animation, Animation::Hasher, Animation::Equal> m_animations;

	detail::AnimationPlayData m_current;
	lsd::Vector<detail::AnimationPlayData> m_queue;

	lsd::StringView m_autoPlay;

	etcs::ComponentView<Sprite> m_sprite;

	SpriteAnimator(
		const etcs::ComponentView<Sprite>& sprite, 
		const lsd::Vector<Animation>& animations, 
		const lsd::String& autoPlay
	);

	void update(std::uint64_t deltaTime);

	void updateSpritePos();

	friend class AnimatorBuilder;
	friend class Application;
};

} // namespace esengine
