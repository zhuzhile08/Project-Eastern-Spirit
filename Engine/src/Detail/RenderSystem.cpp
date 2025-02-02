#include <Detail/RenderSystem.h>

#include <Graphics/Window.h>
#include <Graphics/Texture.h>

namespace esengine {

namespace globals {

detail::RenderSystem* renderSystem = nullptr;

} // namespace globals

namespace detail {

RenderSystem::RenderSystem() {
	insertPass({ }); // default render pass;
}

void RenderSystem::insertPass(lsd::StringView name, Texture* target, std::size_t sortingFactor) {
	auto renderer = m_renderPasses.emplace(RenderPass { 
		sdl::Renderer(globals::window->window()), 
		lsd::String(name),
		sortingFactor,
		target
	}).first->renderer.get();

	if (target) SDL_SetRenderTarget(renderer, target->texture());

#ifndef ESENGINE_DYNAMIC_WINDOW_SIZE
	SDL_SetRenderLogicalPresentation(renderer, globals::window->baseSize().x, globals::window->baseSize().y, SDL_LOGICAL_PRESENTATION_LETTERBOX); // @todo investigate better options
#endif
}

void RenderSystem::removePass(lsd::StringView name) {
	m_renderPasses.erase(name);
}

void RenderSystem::insertCall(const CallData& callData, lsd::StringView passName) {
	auto& renderPass = m_renderPasses.at(passName);

	renderPass.drawData[static_cast<int>(callData.sortingFactor * renderPass.sortingFactor)].emplaceBack(std::move(callData));
}

void RenderSystem::drawPass(lsd::StringView name) {
	auto& pass = m_renderPasses.at(name);

	SDL_RenderClear(pass.renderer.get());

	for (auto& [_, data] : pass.drawData) {
		for (const auto& call : data) {
			if (call.rotation == 0) SDL_RenderTexture(pass.renderer, call.texture, &call.src, &call.dst);
			else SDL_RenderTextureRotated(pass.renderer, call.texture, &call.src, &call.dst, 0, nullptr, SDL_FLIP_NONE);

			data.clear();
		}
	}

	SDL_RenderPresent(pass.renderer);
}

void RenderSystem::drawAll() {
	for (auto& pass : m_renderPasses) {
		SDL_RenderClear(pass.renderer);

		for (auto& [_, data] : pass.drawData) {
			for (const auto& call : data) {
				SDL_RenderTextureRotated(pass.renderer, call.texture, &call.src, &call.dst, 0, nullptr, SDL_FLIP_NONE);

				data.clear();
			}
		}

		SDL_RenderPresent(pass.renderer);
	}
}

} // namespace detail

} // namespace esengine
