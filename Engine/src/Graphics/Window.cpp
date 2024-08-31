#include <Graphics/Window.h>

namespace esengine {

namespace globals {

Window* window = nullptr;

}

Window::Window(lsd::StringView title, const glm::uvec2& size, int flags) {
	if (m_window = sdl::Window(title, size.x, size.y, flags); !m_window) throw std::runtime_error(SDL_GetError());

	SDL_SetWindowRelativeMouseMode(m_window, SDL_TRUE);
}

glm::uvec2 Window::windowSize() const {
	glm::ivec2 r;
	SDL_GetWindowSizeInPixels(m_window, &r.x, &r.y);

	return r;
}

} // namespace esengine
