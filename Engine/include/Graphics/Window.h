/*************************
 * @file Window.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 *
 * @brief Wrapper around SDL window class
 *
 * @date 2024-08-31
 *
 * @copyright Copyright (c) 2024
 *************************/

#pragma once

#include <Common/RAIIContainers.h>

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

namespace esengine {

class Window {
public:
	Window(lsd::StringView title, const glm::ivec2& size, int flags);

	const glm::ivec2& size() const noexcept {
		return m_size;
	}
	[[nodiscard]] bool changed() const noexcept {
		return m_changed;
	}

private:
	sdl::Window m_window;

	glm::ivec2 m_size;
	bool m_changed = false;


	friend class InputSystem;
};

} // namespace esengine
