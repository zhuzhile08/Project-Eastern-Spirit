#include <Detail/RenderSystem.h>

#include <Graphics/Window.h>
#include <Graphics/Texture.h>

namespace esengine {

namespace globals {

detail::RenderSystem* renderSystem = nullptr;

} // namespace globals

namespace detail {

RenderSystem::RenderSystem() : m_renderer(globals::window->window()) {
#ifndef ESENGINE_DYNAMIC_WINDOW_SIZE
	SDL_SetRenderLogicalPresentation(m_renderer, globals::window->baseSize().x, globals::window->baseSize().y, SDL_LOGICAL_PRESENTATION_LETTERBOX); // @todo investigate better options
#endif
}

void RenderSystem::insertPass(
	lsd::StringView name, 
	SDL_Texture* target, 
	const SDL_FRect& viewport,
	const SDL_FRect& clipRect
) {
	auto rpIt = m_renderPasses.find(name);

	if (rpIt == m_renderPasses.end())
		m_renderPasses.emplace(name, RenderPass {
			.target = target,
			.viewport = viewport,
			.clipRect = clipRect
		});
}

void RenderSystem::removePass(lsd::StringView name) {
	m_renderPasses.erase(name);
}

void RenderSystem::draw() {
	// Clear the screen to black
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_renderer);

#ifdef ESENGINE_DYNAMIC_WINDOW_SIZE
	glm::ivec2 renderSize = globals::window()->size();
#else
	glm::ivec2 renderSize;
	SDL_GetRenderLogicalPresentation(m_renderer, &renderSize.x, &renderSize.y, nullptr);
#endif

	for (auto& [_, pass] : m_renderPasses) {
		SDL_SetRenderTarget(m_renderer, pass.target);

		SDL_Rect rect = { 
			static_cast<int>(pass.viewport.x * renderSize.x), static_cast<int>(pass.viewport.y * renderSize.y), 
			static_cast<int>(pass.viewport.w * renderSize.x), static_cast<int>(pass.viewport.h * renderSize.y)
		};
		SDL_SetRenderViewport(m_renderer, &rect);

		rect = { 
			static_cast<int>(pass.clipRect.x * renderSize.x), static_cast<int>(pass.clipRect.y * renderSize.y), 
			static_cast<int>(pass.clipRect.w * renderSize.x), static_cast<int>(pass.clipRect.h * renderSize.y)
		};
		SDL_SetRenderClipRect(m_renderer, &rect);

		for (auto& [_, data] : m_drawData) {
			for (const auto& call : data)
				SDL_RenderTextureRotated(m_renderer, call.texture, &call.src, &call.dst, 0, nullptr, SDL_FLIP_NONE);

			data.clear();
		}
	}
	
	SDL_RenderPresent(m_renderer);
}

} // namespace detail

} // namespace esengine
