#include <InputSystem.h>

#include <Graphics/Window.h>

#include <SDL3/SDL.h>

#include <LSD/UnorderedSparseMap.h>
#include <LSD/Operators.h>

using namespace lsd::enum_operators;


namespace esengine {

namespace globals {

InputSystem* inputSystem = nullptr;

extern Window* window;

} // namespace globals

InputSystem::InputSystem() { 
	m_mouseState = SDL_GetMouseState(&m_mousePos.x, &m_mousePos.y);
	m_keyboardState = SDL_GetKeyboardState(nullptr);
}

bool InputSystem::quit() {
	return m_quit;
}
void InputSystem::cancelQuit() {
	m_quit = false;
}

input::Key InputSystem::keyboard(input::KeyType type) {
	return m_keys[type];
}
input::MouseButton InputSystem::mouse(input::MouseButtonType type) {
	return m_mouseButtons[type];
}
input::ControllerButton InputSystem::controller(input::ControllerButtonType type) {
	return m_controllerButtons[type];
}
glm::vec2 InputSystem::mousePos() {
	return m_mousePos;
}
glm::vec2 InputSystem::mouseDelta() {
	return m_mouseDelta;
}
glm::vec2 InputSystem::analogueStickPos() {
	return m_stickPos;
}

void InputSystem::update() {
	globals::window->m_changed = false;
	m_mouseDelta = { 0, 0 };
	
	SDL_PumpEvents();
	
	for (auto& button : m_controllerButtons) {
		button.second.reset();
	}
	
	auto mState = SDL_GetMouseState(&m_mousePos.x, &m_mousePos.y);
	for (auto& button : m_mouseButtons) {
		button.second.reset();
		
		if ((m_mouseState & button.first) == button.first) button.second.held = true;
		std::swap(mState, m_mouseState);
	}

	auto kState = SDL_GetKeyboardState(nullptr);
	for (auto& key : m_keys) {
		key.second.reset();
		
		if (m_keyboardState[static_cast<std::size_t>(key.first)] && kState[static_cast<std::size_t>(key.first)]) key.second.held = true;
		std::swap(kState, m_keyboardState);
	}

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		// implement controller events @todo

		// default events that are always checked
		switch (event.type) {
			case SDL_EVENT_QUIT:
				m_quit = true;
			case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
			case SDL_EVENT_WINDOW_MINIMIZED:
			case SDL_EVENT_WINDOW_MAXIMIZED:
				globals::window->m_changed = true;
			case SDL_EVENT_KEY_DOWN:
				for (auto& key : m_keys) {
					if (event.key.key == key.first) key.second.pressed = true;
				}
			case SDL_EVENT_KEY_UP:
				for (auto& key : m_keys) {
					if (event.key.key == key.first) key.second.released = true;
				}
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				for (auto& key : m_mouseButtons) {
					if (event.button.button == key.first) key.second.pressed = true;
				}
			case SDL_EVENT_MOUSE_BUTTON_UP:
				for (auto& key : m_mouseButtons) {
					if (event.button.button == key.first) key.second.released = true;
				}
			case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
				for (auto& key : m_controllerButtons) {
					if (event.button.button == key.first) key.second.pressed = true;
				}
			case SDL_EVENT_GAMEPAD_BUTTON_UP:
				for (auto& key : m_controllerButtons) {
					if (event.button.button == key.first) key.second.released = true;
				}

			case SDL_EVENT_MOUSE_MOTION:
				m_mouseDelta = { event.motion.xrel, event.motion.yrel };
		}
	}
}

} // namespace esengine
