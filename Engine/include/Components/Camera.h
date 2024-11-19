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

#include <Common/Common.h>

#include <ETCS/Component.h>
#include <ETCS/Components/Transform.h>

#include <LSD/String.h>
#include <LSD/StringView.h>

#include <SDL3/SDL.h>

#include <glm/glm.hpp>

namespace esengine {

class Camera {
public:
	Camera(lsd::StringView passName = { });
	Camera(SDL_FRect viewport, lsd::StringView passName = { });

	[[nodiscard]] glm::mat4 transformMat(const etcs::Entity& camera, const etcs::Transform& transform) const noexcept;

	[[nodiscard]] const lsd::String& passName() const noexcept {
		return m_passName;
	}

	SDL_FRect viewport;
	SDL_FRect limits { -10000000.f, 10000000.f, 2 * 10000000.f, 2 * 10000000.f };
	glm::vec2 offset { };
	glm::vec2 zoom { 1, 1 };

	etcs::ComponentView<etcs::Transform> target;

private:
	lsd::String m_passName;

	void update(etcs::Transform& transform);

	friend class Application;
};

} // namespace esengine
