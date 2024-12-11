/*************************
 * @file InputSystem.h
 * @author zhuzhile08 (zhuzhile08@gmail.com)
 * 
 * @brief A wrapper around SDL input which polls events
 * 
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2022
 *************************/

#pragma once

#include <Core/Common.h>
#include <Core/Input.h>

#include <LSD/Operators.h>
#include <LSD/Hash.h>
#include <LSD/UnorderedSparseSet.h>

#include <glm/glm.hpp>

namespace esengine {

namespace detail {

class InputSystem {
public:
	InputSystem();

	bool quit();
	void cancelQuit();

	const Key& keyboard(KeyType type);
	const MouseButton& mouse(MouseButtonType type);
	const ControllerButton& controller(ControllerButtonType type);
	glm::vec2 mousePos();
	glm::vec2 mouseDelta();
	glm::vec2 analogueStickPos();

	void startFrame();
	void update();

private:
	lsd::UnorderedSparseSet<Key, input_system_detail::KHasher, input_system_detail::KEqual> m_keys;
	lsd::UnorderedSparseSet<MouseButton, input_system_detail::MBHasher, input_system_detail::MBEqual> m_mouseButtons;
	lsd::UnorderedSparseSet<ControllerButton, input_system_detail::CBHasher, input_system_detail::CBEqual> m_controllerButtons;

	bool m_quit = false;

	std::uint32_t m_mouseState;
	const bool* m_keyboardState;

	glm::vec2 m_mousePos;
	glm::vec2 m_mouseDelta;
	glm::vec2 m_stickPos;
	glm::vec2 m_stickDelta;
};

} // namespace detail

} // namespace esengine
