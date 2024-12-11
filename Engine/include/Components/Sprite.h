/************************
 * @file Sprite.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Sprite lcass
 * 
 * @date 2024-09-06
 * 
 * @copyright Copyright (c) 2024
 ************************/

#pragma once

#include <Core/Common.h>
#include <Detail/RenderSystem.h>
#include <Graphics/Texture.h>

#include <LSD/String.h>

#include <ETCS/Component.h>
#include <ETCS/Components/Transform.h>

#include <SDL3/SDL.h>

#include <glm/glm.hpp>

namespace esengine {

class Sprite {
public:
	Sprite() = default;
	Sprite(SDL_FRect rect, Texture* texture) : m_rect(rect), m_texture(texture) { }

	[[nodiscard]] bool drawCall(
		detail::RenderSystem::CallData& data,
		const etcs::Entity& entity,
		const etcs::Transform& transform,
		const glm::mat4& camTransform,
		const Camera& camera) const;

private:
	SDL_FRect m_rect { };
	Texture* m_texture { };

	friend class SpriteAnimator;
};

} // namespace esengine
