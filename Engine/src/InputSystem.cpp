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

const Key& InputSystem::keyboard(KeyType type) {
	auto& k = m_keys[type];
	k.type = type;
	return k;
}

const MouseButton& InputSystem::mouse(MouseButtonType type) {
	auto& b = m_mouseButtons[type];
	b.type = type;
	return b;
}

const ControllerButton& InputSystem::controller(ControllerButtonType type) {
	auto& b = m_controllerButtons[type];
	b.type = type;
	return b;
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

void InputSystem::startFrame() {
	globals::window->m_changed = false;
}

void InputSystem::update() {
	m_mouseDelta = { 0, 0 };
	
	SDL_PumpEvents();
	
	for (auto& button : m_controllerButtons) {
		button.reset();
	}
	
	auto mState = SDL_GetMouseState(&m_mousePos.x, &m_mousePos.y);
	for (auto& button : m_mouseButtons) {
		button.reset();
		
		if ((m_mouseState & button.type) == button.type) button.held = true;
		std::swap(mState, m_mouseState);
	}

	auto kState = SDL_GetKeyboardState(nullptr);
	for (auto& key : m_keys) {
		key.reset();
		
		if (m_keyboardState[static_cast<std::size_t>(key.type)] && kState[static_cast<std::size_t>(key.type)]) key.held = true;
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
			case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
			case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
				globals::window->m_changed = true;
				SDL_GetWindowSizeInPixels(globals::window->m_window, &globals::window->m_size.x, &globals::window->m_size.y);
			case SDL_EVENT_KEY_DOWN:
				for (auto& key : m_keys) {
					if (event.key.key == key.type) key.pressed = true;
				}
			case SDL_EVENT_KEY_UP:
				for (auto& key : m_keys) {
					if (event.key.key == key.type) key.released = true;
				}
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				for (auto& key : m_mouseButtons) {
					if (event.button.button == key.type) key.pressed = true;
				}
			case SDL_EVENT_MOUSE_BUTTON_UP:
				for (auto& key : m_mouseButtons) {
					if (event.button.button == key.type) key.released = true;
				}
			case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
				for (auto& key : m_controllerButtons) {
					if (event.button.button == key.type) key.pressed = true;
				}
			case SDL_EVENT_GAMEPAD_BUTTON_UP:
				for (auto& key : m_controllerButtons) {
					if (event.button.button == key.type) key.released = true;
				}

			case SDL_EVENT_MOUSE_MOTION:
				m_mouseDelta = { event.motion.xrel, event.motion.yrel };
		}
	}
}

} // namespace esengine
