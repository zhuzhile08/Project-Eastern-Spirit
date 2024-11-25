#include <Graphics/Window.h>

namespace esengine {

namespace globals {

Window* window = nullptr;

}

#ifdef ESENGINE_DYNAMIC_WINDOW_SIZE

Window::Window(lsd::StringView title, const glm::ivec2& size, int flags) : m_size(size) {
	if (m_window = sdl::Window(title, size.x, size.y, flags); !m_window) throw std::runtime_error(SDL_GetError());

	SDL_SetWindowRelativeMouseMode(m_window, true);
}

void Window::resize(const glm::ivec2& size) {
	m_size = size;

	SDL_SetWindowSize(m_window, m_size.x, m_size.y);
}

#else

Window::Window(lsd::StringView title, const glm::ivec2& baseSize, std::size_t scale, int flags) : m_baseSize(baseSize), m_scale(scale) {
	if (m_window = sdl::Window(title, m_baseSize.x * m_scale, m_baseSize.y * m_scale, flags); !m_window) throw std::runtime_error(SDL_GetError());

	SDL_SetWindowRelativeMouseMode(m_window, true);
}

void Window::resize(std::size_t scale) {
	m_scale = scale;

	SDL_SetWindowSize(m_window, m_baseSize.x * scale, m_baseSize.y * scale);
}

void Window::setFocus(bool focus) {
	SDL_SetWindowRelativeMouseMode(m_window, focus);
}

bool Window::focus() {
	SDL_GetWindowRelativeMouseMode(m_window);
}

#endif

} // namespace esengine
