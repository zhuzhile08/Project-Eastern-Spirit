/*************************
 * @file TextBoxAnimator.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Animator for the textbox
 * 
 * @date 2024-12-17
 * @copyright Copyright (c) 2024
 *************************/

#pragma once

#include <Core/Common.h>

#include <Components/TextBox.h>

#include <ETCS/Component.h>

#include <cstddef>
#include <chrono>

namespace esengine {

class TextBoxAnimator {
public:
	TextBoxAnimator(const etcs::ComponentView<TextBox>& textBox, ms_time_t charTime = constants::defaultTextSpeed);

	void skip();
	void restart();

	void update(ms_time_t dt);

	[[nodiscard]] const etcs::ComponentView<TextBox>& textBox() {
		return m_textBox;
	}

	float speed = 1.0f;

private:
	const ms_time_t m_charTime;
	ms_time_t m_elapsed = 0.0f;

	etcs::ComponentView<TextBox> m_textBox;
};

} // namespace esengine
