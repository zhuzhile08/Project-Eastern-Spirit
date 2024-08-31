#include <Graphics/Window.h>

namespace esengine {

namespace globals {

Window* window = nullptr;

}

Window::Window(lsd::StringView title, const glm::ivec2& size, int flags) : m_size(size) {
	if (m_window = sdl::Window(title, size.x, size.y, flags); !m_window) throw std::runtime_error(SDL_GetError());

	SDL_SetWindowRelativeMouseMode(m_window, SDL_TRUE);
}

} // namespace esengine
