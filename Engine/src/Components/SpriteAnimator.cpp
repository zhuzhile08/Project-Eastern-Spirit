#include <Components/SpriteAnimator.h>

#include <Graphics/Texture.h>

#include <SDL3/SDL.h>

namespace esengine {

SpriteAnimator::SpriteAnimator(const etcs::ComponentView<Sprite>& sprite, Animation&& autoPlay) : m_sprite(sprite) {
	m_autoPlay = m_animations.emplace(autoPlay).first->name;

	m_current = AnimPlayData { m_autoPlay, 0, 0, 1.f };
}

void SpriteAnimator::play(lsd::StringView animationName, float speed) {
	auto it = m_animations.find(animationName);

	if (it == m_animations.end()) {
		throw std::out_of_range("esengine::SpriteAnimator::play(): Requested animation to play didn't exist!");
	} else {
		m_current = AnimPlayData { it->name, 0, 0, speed };
	}
}

void SpriteAnimator::setFrame(std::size_t frame, std::uint64_t startingTime) {
	if (m_current.playing()) {
		m_current.frameIndex = frame;
		m_current.frameTime = startingTime;
	}
}

void SpriteAnimator::queue(lsd::StringView animationName, float speed) {
	auto it = m_animations.find(animationName);

	if (it == m_animations.end()) {
		throw std::out_of_range("esengine::SpriteAnimator::queue(): Requested animation to add to queue didn't exist!");
	} else {
		m_queue.emplace(AnimPlayData { it->name, 0, 0, speed });
	}
}

void SpriteAnimator::update(std::uint64_t deltaTime) {
	if (m_current.playing()) {
		const auto& anim = m_animations.at(m_current.name);
		const auto& frame = anim.frames[m_current.frameIndex];
		auto& sprite = m_sprite.get();

		m_current.frameTime += deltaTime;

		if (m_current.frameTime >= lsd::implicitCast<std::uint64_t>(frame.time.count())) {
			m_current.frameTime %= frame.time.count();

			m_current.frameIndex += 1;

			if (m_current.frameIndex >= anim.frames.size()) {
				if (!anim.repeated) {
					m_current = AnimPlayData();

					return;
				} else m_current.frameIndex = 0;
			}

			const auto& newFrame = anim.frames[m_current.frameIndex];

			auto framesPerRow = sprite.m_texture->dimension().x / lsd::implicitCast<std::size_t>(sprite.m_rect.w);

			sprite.m_rect.x = (newFrame.index % framesPerRow) * sprite.m_rect.w;
			sprite.m_rect.y = (newFrame.index / framesPerRow) * sprite.m_rect.h;
		}
	}
}

} // namespace esengine
