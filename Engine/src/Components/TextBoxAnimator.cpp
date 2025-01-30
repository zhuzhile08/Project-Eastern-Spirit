#include <Components/TextBoxAnimator.h>

namespace esengine {

TextBoxAnimator::TextBoxAnimator(const etcs::ComponentView<TextBox>& textBox, std::chrono::milliseconds charTime) : 
	m_charTime(charTime.count()),
	m_textBox(textBox) {
	m_textBox.get().m_index = 0;
}

void TextBoxAnimator::skip() {
	speed = 0.0f;
	m_textBox.get().m_index = std::numeric_limits<std::size_t>::max();
}

void TextBoxAnimator::restart() { 
	speed = 1.0f;
	m_elapsed = 0.0f;
	m_textBox.get().m_index = 0;
}

void TextBoxAnimator::update(es_time_t dt) {
	auto& textBox = m_textBox.get();

	if (textBox.m_index < textBox.text.size()) {
		m_elapsed += dt * speed;

		if (m_elapsed >= m_charTime) {
			m_elapsed -= m_charTime;
			++textBox.m_index;
		}
	}
}

} // namespace esengine
