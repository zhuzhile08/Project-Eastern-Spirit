#include <Graphics/RenderSystem.h>

#include <Graphics/Window.h>
#include <Graphics/Texture.h>

namespace esengine {

namespace globals {

RenderSystem* renderSystem = nullptr;

} // namespace globals

RenderSystem::RenderSystem() {
	insertPass({ }); // default render pass;
}

void RenderSystem::insertPass(lsd::StringView name, Texture* target) {
	auto renderer = m_renderPasses.emplace(RenderPass { sdl::Renderer(globals::window->window()), name }).first->renderer.get();

	if (target) SDL_SetRenderTarget(renderer, target->texture());
}

void RenderSystem::removePass(lsd::StringView name) {
	m_renderPasses.erase(name);
}

void RenderSystem::insertCall(double yPos, CallData&& callData) {
	m_renderPasses.at(callData.passName).drawData[yPos * 10].emplaceBack(std::move(callData));
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

} // namespace esengine
