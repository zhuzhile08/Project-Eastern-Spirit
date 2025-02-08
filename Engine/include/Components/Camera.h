/************************
 * @file Camera.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Simple camera class
 * 
 * @date 2024-09-10
 * 
 * @copyright Copyright (c) 2024
 ************************/

#pragma once

#include <Core/Common.h>

#include <Detail/RenderSystem.h>

#include <ETCS/Component.h>
#include <ETCS/Components/Transform.h>

#include <LSD/String.h>
#include <LSD/StringView.h>

#include <SDL3/SDL.h>

#include <glm/glm.hpp>

namespace esengine {

class Camera {
public:
	Camera(lsd::StringView passName = { }, SDL_Texture* target = nullptr);
	Camera(const SDL_FRect& viewport, lsd::StringView passName = { }, SDL_Texture* target = nullptr);

	void destoryRenderPass() const;

	void setRenderPassClipRect(const SDL_FRect& clipRect) const;
	void setRenderPassViewport(const SDL_FRect& viewport) const;

	[[nodiscard]] SDL_FRect renderPassClipRect() const;
	[[nodiscard]] SDL_FRect renderPassViewport() const;

	glm::mat4 renderMatrix(const etcs::Entity& entity, etcs::Transform& transform);

	[[nodiscard]] bool validRenderPass() const noexcept;
	[[nodiscard]] const lsd::String& passName() const noexcept {
		return m_passName;
	}

	SDL_FRect limits { -10000000.f, 10000000.f, 2 * 10000000.f, 2 * 10000000.f };
	glm::vec2 offset { };
	glm::vec2 zoom { 1, 1 };

	etcs::Entity target;

private:
	lsd::String m_passName;
};

} // namespace esengine
