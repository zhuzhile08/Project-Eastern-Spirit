/*************************
 * @file AnimationBuilder.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Basic classes for the animation system
 * 
 * @date 2024-11-24
 * @copyright Copyright (c) 2024
 *************************/

#pragma once

#include <Core/Common.h>

#include <LSD/Vector.h>
#include <LSD/String.h>
#include <LSD/StringView.h>

#include <chrono>
#include <cstddef>

namespace esengine {

namespace detail {

struct Frame {
	std::size_t index;
	std::chrono::milliseconds time = constants::defaultAnimationFrameTime;
};

struct AnimationPlayData {
public:
	[[nodiscard]] bool playing() const noexcept {
		return !name.empty() && speed > 0.f;
	}

	lsd::StringView name { };
	const Animation* animation { };
	bool singleFrame { };

	std::size_t frameIndex { };
	std::uint64_t frameTime { };
	float speed = 1.f;
};

} // namespace detail

struct Animation {
public:
	CUSTOM_HASHER(Hasher, const Animation&, const lsd::String&, lsd::Hash<lsd::String>(), .name)
	CUSTOM_EQUAL(Equal, const Animation&, const lsd::String&, .name)

	lsd::String name;
	lsd::Vector<detail::Frame> frames;

	bool repeated = false;
};

} // namespace esengine
