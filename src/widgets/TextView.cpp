#include "Widget.h"


ui::TextView::TextView(const ci::XmlTree& node) : InputBox(node) {
	m_text = node.getValue();
}

void ui::TextView::render() {
	ui::InputBox::render();
}

void ui::TextView::mouseDown(ci::app::MouseEvent& event) {
	ui::InputBox::mouseDown(event);
}

void ui::TextView::keyDown(ci::app::KeyEvent& event) {
	ui::InputBox::keyDown(event);

	if (isProperChar(event.getCode())) {
		if (event.getCode() == 9) {
			m_text.push_back(' ');
			m_text.push_back(' ');
			m_text.push_back(' ');
			m_text.push_back(' ');
			m_text.push_back(' ');
		} else if (event.getCode() == 13) {
			m_text.push_back('\n');
		}
	}
}

const bool ui::TextView::isProperChar(const int& i) {
	if (i >= 32 && i <= 126 || i == 9 || i == 13) {// enter and tab
		return true;
	}
	return false;
}
