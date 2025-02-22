/*************************
 * @file RenderSystem.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Rendering backend
 * 
 * @date 2024-09-01
 * 
 * @copyright Copyright (c) 2024
 *************************/

#pragma once

#include <Core/Common.h>
#include <Core/RAIIContainers.h>

#include <SDL3/SDL.h>

#include <LSD/Vector.h>
#include <LSD/String.h>
#include <LSD/UnorderedSparseMap.h>

#include <glm/glm.hpp>

#include <map>

namespace esengine {

namespace detail {

class RenderSystem {
public:
	struct CallData {
	public:
		double rotation;
		
		SDL_FRect src;
		SDL_FRect dst;

		SDL_Texture* texture;
	};

private:
	struct RenderPass {
	public:
		SDL_Texture* target = nullptr;

		SDL_FRect viewport = { 0, 0, 1, 1 };
		SDL_FRect clipRect = { 0, 0, 1, 1 };
	};

public:
	RenderSystem();

	void insertPass(
		lsd::StringView name, 
		SDL_Texture* target, 
		const SDL_FRect& viewport,
		const SDL_FRect& clipRect
	);
	void removePass(lsd::StringView name);
	
	void draw();

	[[nodiscard]] bool containsPass(lsd::StringView name = { }) const noexcept {
		return m_renderPasses.contains(name);
	}

	[[nodiscard]] SDL_Renderer* renderer() noexcept {
		return m_renderer;
	}
	[[nodiscard]] const RenderPass& pass(lsd::StringView name = { }) const {
		return m_renderPasses.at(name);
	}
	[[nodiscard]] RenderPass& pass(lsd::StringView name = { }) {
		return m_renderPasses.at(name);
	}
	[[nodiscard]] decltype(auto) drawData(int zCoord) noexcept {
		return (m_drawData[zCoord]);
	}

private:
	sdl::Renderer m_renderer;

	lsd::UnorderedSparseMap<lsd::String, RenderPass> m_renderPasses;
	std::map<int, lsd::Vector<CallData>, std::greater<int>> m_drawData;
};

} // namespace detail

} // namespace esengine
