#include <Components/TextBox.h>

namespace esengine {

void TextBox::changeText(lsd::StringView newText) {
	m_text = newText;
};

} //namespace esengine
