/*************************
 * @file AnimatorBuilder.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief A class to assist in the construction of new animators using the builder pattern
 * 
 * @date 2024-11-24
 * @copyright Copyright (c) 2024
 *************************/

#pragma once

#include <Common/Common.h>
#include <Common/Animation.h>

#include <Components/Sprite.h>

#include <ETCS/Component.h>

#include <LSD/StringView.h>

namespace esengine {

class AnimatorBuilder {
public:
	void addAnimation(Animation&& anim);
	void addAutoPlay(Animation&& anim);

	SpriteAnimator buildSpriteAnimator(const etcs::ComponentView<Sprite>& sprite) const;

private:
	lsd::Vector<Animation> m_animations;
	lsd::StringView m_autoPlay;

	bool verifyAnimation(Animation&& anim) const;
};

} // namespace esengine
