#include <Components/TextBoxAnimator.h>

namespace esengine {

TextBoxAnimator::TextBoxAnimator(const etcs::ComponentView<TextBox>& textBox, ms_time_t charTime) : 
	m_charTime(charTime),
	m_textBox(textBox) {
	m_textBox.get().m_renderLimit = 0;
}

void TextBoxAnimator::skip() {
	speed = 0.0f;
	m_textBox.get().m_renderLimit = std::numeric_limits<std::size_t>::max();
}

void TextBoxAnimator::restart() { 
	speed = 1.0f;
	m_elapsed = 0.0f;
	m_textBox.get().m_renderLimit = 0;
}

void TextBoxAnimator::update(ms_time_t dt) {
	auto& textBox = m_textBox.get();

	if (textBox.m_renderLimit <= textBox.m_text.size()) {
		m_elapsed += dt * speed;

		if (m_elapsed >= m_charTime) {
			m_elapsed -= m_charTime;
			
			++textBox.m_renderLimit;
			textBox.m_dirty = true;
		}
	}
}

} // namespace esengine
