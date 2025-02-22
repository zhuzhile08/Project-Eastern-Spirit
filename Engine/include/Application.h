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

#include <Core/Common.h>

#include <SDL3/SDL.h>

#include <LSD/StringView.h>
#include <LSD/FunctionPointer.h>

#include <glm/glm.hpp>

namespace esengine {

struct InitInfo {
	char** argv;

	lsd::StringView name;
	glm::ivec2 dim;

#ifdef ESENGINE_DYNAMIC_WINDOW_SIZE
	int flags = SDL_WINDOW_RESIZABLE;
#else
	int flags = { };
#endif

	ms_time_t deltaTime = 33;
};


class Application {
public:
	Application(InitInfo info);
	~Application();

	void run();
	[[nodiscard]] virtual double fps() const noexcept final {
		return 1000.0f / (m_currentTime - m_startTime);
	}

protected:
	virtual void update(float) { }

	ms_time_t m_accumulator = 0;
	ms_time_t m_deltaTime;

	ms_time_t m_startTime = 0;
	ms_time_t m_currentTime = 0;
};

} // namespace esengine
