/************************
 * @file 3DSprite.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Sprite with perspective rendering
 * 
 * @date 2024-09-06
 * 
 * @copyright Copyright (c) 2024
 ************************/

#pragma once

#include <Common/Common.h>
#include <Graphics/RenderSystem.h>
#include <Graphics/Texture.h>

#include <ETCS/Component.h>

#include <SDL3/SDL.h>

#include <LSD/String.h>

#include <glm/glm.hpp>

namespace esengine {

class Sprite3D {
public:
	Sprite3D() = default;
	Sprite3D(SDL_FRect rect, Texture* texture) : m_rect(rect), m_texture(texture) { }

	[[nodiscard]] RenderSystem::CallData drawCall(const glm::vec3& translation, const glm::mat4& camTransform, const Camera& camera) const;

private:
	SDL_FRect m_rect { };
	Texture* m_texture { };

	friend class Animator;
};

} // namespace esengine
