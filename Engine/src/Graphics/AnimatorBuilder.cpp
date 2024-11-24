#include <Graphics/AnimatorBuilder.h>

#include <Graphics/Texture.h>

#include <Components/SpriteAnimator.h>

#include <cassert>

namespace esengine {

void AnimatorBuilder::addAnimation(Animation&& anim) {
#ifndef NDEBUG
	assert((verifyAnimation(std::move(anim))) && "esengine::AnimatorBuilder::addAnimation(): An animation frame can't be zero milliseconds long!");
#endif
	m_animations.emplaceBack(std::move(anim));
}

void AnimatorBuilder::addAutoPlay(Animation&& anim) {
#ifndef NDEBUG
	assert((verifyAnimation(std::move(anim))) && "esengine::AnimatorBuilder::addAnimation(): An animation frame can't be zero milliseconds long!");
#endif
	m_autoPlay = m_animations.emplaceBack(std::move(anim)).name;
}

SpriteAnimator AnimatorBuilder::buildSpriteAnimator(const etcs::ComponentView<Sprite>& sprite) const {
	return SpriteAnimator(sprite, m_animations, m_autoPlay);
}

bool AnimatorBuilder::verifyAnimation(Animation&& anim) const {
	for (const auto& frame : anim.frames) if (frame.time.count() == 0) return false;
	return true;
}

} // namespace esengine
