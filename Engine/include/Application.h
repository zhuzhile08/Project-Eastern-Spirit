/*************************
 * @file Application.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Application class
 * 
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 *************************/

#pragma once

#include <Common/Common.h>

#include <SDL3/SDL.h>

#include <ETCS/Components/Transform.h>
#include <ETCS/System.h>

#include <LSD/StringView.h>
#include <LSD/FunctionPointer.h>

#include <glm/glm.hpp>

namespace esengine {

struct InitInfo {
	char** argv;

	lsd::StringView name = { };
	glm::ivec2 dim = { 1280, 720 };
	int flags = { };

	std::uint64_t deltaTime = 16;
};


class Application {
public:
	Application(InitInfo info);
	~Application();

	virtual void run() final;
	[[nodiscard]] virtual double fps() const noexcept final {
		return 1000.0f / (m_currentTime - m_startTime);
	}

protected:
	virtual void update() { }

	std::uint64_t m_accumulator = 0;
	std::uint64_t m_deltaTime;

	std::uint64_t m_startTime = 0;
	std::uint64_t m_currentTime = 0;

	lsd::Function<void(const etcs::Transform&, const SpriteRect&)> m_projectionFunction;
	etcs::System<const etcs::Transform, const SpriteRect> m_projector;
};

} // namespace esengine
