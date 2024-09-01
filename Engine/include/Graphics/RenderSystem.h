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

#include <Common/RAIIContainers.h>

#include <Common/Common.h>

#include <SDL3/SDL.h>

#include <LSD/Vector.h>
#include <LSD/String.h>
#include <LSD/UnorderedSparseSet.h>

#include <map>

namespace esengine {

class RenderSystem {
public:
	struct CallData {
	public:
		Texture* texture;
		SDL_FRect src;
		SDL_FRect dst;
		double rotation;
	};

private:
	struct RenderPass {
	public:
		sdl::Renderer renderer;
		lsd::String name;

		Texture* target = nullptr;

		std::map<int, lsd::Vector<CallData>, std::greater<float>> drawData = { };

	};

	CUSTOM_HASHER(Hasher, const RenderPass&, const lsd::String&, lsd::Hash<lsd::String>(), .name)
	CUSTOM_EQUAL(Equal, const RenderPass&, const lsd::String&, .name)

public:
	RenderSystem() = default;

	void insertPass(lsd::StringView name/*, Texture* target = nullptr*/);
	void removePass(lsd::StringView name);

	void insertCall(lsd::StringView name, CallData&& callData);
	
	void drawPass(lsd::StringView name);
	void drawAll();

private:
	lsd::UnorderedSparseSet<RenderPass, Hasher, Equal> m_renderPasses;
};

} // namespace esengine
