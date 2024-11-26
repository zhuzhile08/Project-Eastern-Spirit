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

#ifdef ESENGINE_DYNAMIC_WINDOW_SIZE

class Window {
public:
	Window(lsd::StringView title, const glm::ivec2& size, int flags);

	void resize(const glm::ivec2& size);

	[[nodiscard]] SDL_Window* window() const noexcept {
		return m_window.get();
	}
	[[nodiscard]] const glm::ivec2& size() const noexcept {
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

#else

class Window {
public:
	Window(lsd::StringView title, const glm::ivec2& baseSize, std::size_t scale, int flags);

	void resize(std::size_t scale);
	void setFocus(bool focused);

	bool focused();

	[[nodiscard]] SDL_Window* window() const noexcept {
		return m_window.get();
	}
	[[nodiscard]] const glm::ivec2& baseSize() const noexcept {
		return m_baseSize;
	}
	[[nodiscard]] std::size_t scale() const noexcept {
		return m_scale;
	}
	[[nodiscard]] bool changed() const noexcept {
		return m_changed;
	}

private:
	sdl::Window m_window;

	glm::ivec2 m_baseSize;
	std::size_t m_scale;

	bool m_changed = false;

	friend class InputSystem;
};

#endif

} // namespace esengine
