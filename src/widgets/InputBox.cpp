#include "Widget.h"

ui::InputBox::InputBox(const ci::XmlTree& node) : Label(node) {}

void ui::InputBox::render() {
	m_text_cursor->m_position = ci::Vec2f(m_position.x + 4, m_position.y);
	m_textbox = ci::TextBox();
	m_textbox.size(m_size);
	if (m_text.size() >= DefaultStringSize) {
		if (m_current_char >= m_text.size() - DefaultStringSize) {
			m_textbox.text(m_text.substr(m_text.size() - DefaultStringSize, DefaultStringSize)).font(ci::Font(DefaultFont, m_font_size));
		} else {
			m_textbox.text(m_text.substr(m_current_char, DefaultStringSize)).font(ci::Font(DefaultFont, m_font_size));
		}
	} else {
		m_textbox.text(m_text).font(ci::Font(DefaultFont, m_font_size));
	}
	m_textbox.color(ci::Color(BlackColor)).backgroundColor(ci::Color(WhiteColor));
	m_textbox.alignment(m_alignment);
	m_texture = ci::gl::Texture(m_textbox.render());
}

void ui::InputBox::draw() {
	ui::Widget::draw();
	m_text_cursor->draw();
}

void ui::InputBox::mouseDown(ci::app::MouseEvent& event) {
	if (event.isLeftDown() && withinWidget(event.getPos())) {
		m_is_writable = true;
	} else {
		m_is_writable = false;
	}
	m_current_char = calculateCharPosition(event.getPos());
}

void ui::InputBox::keyDown(ci::app::KeyEvent& event) {
	if (m_is_writable) {
		if (event.getCode() == event.KEY_BACKSPACE) {
			if (m_current_char > m_text.size()) {
				m_current_char = m_text.size();
			}
			if (m_text.size() > 0) {
				m_text = m_text.substr(0, m_current_char - 1) + m_text.substr(m_current_char, m_text.size());
			}
			return;
		} else if (event.getCode() == event.KEY_LEFT) {
			if (m_current_char > 0) {
				m_current_char--;
			}
		} else if (event.getCode() == event.KEY_RIGHT) {
			if (m_current_char < m_text.size()) {
				m_current_char++;
			}
		}
		if (isProperChar(event.getCode()) && (event.getCode() != 9 && event.getCode() != 13)) {
			if (m_current_char + 1 < m_text.size()) {
				m_text.insert(m_current_char + 1, 1, event.getChar());
			} else {
				m_text.push_back(event.getChar());
			}
			m_current_char++;
		}
	}
}

const bool ui::InputBox::isProperChar(const int& i) {
	if (i >= 32 && i <= 126) { // normal chars
		return true;
	}
	return false;
}

const int ui::InputBox::calculateCharPosition(const ci::Vec2f& position) {
	if (withinWidget(position)) {
		if (m_text == "") {
			return 0;
		}
		int pos = (position.x - m_position.x - 4) / (g_font_point_map.at(m_font_size) / 5);
		float pos_y = m_text_cursor->m_position.y;
		float pos_x = position.x - m_position.x - 4;
		pos_x = pos_x - ((int)pos_x % 5);
		m_text_cursor->m_position = ci::Vec2f(pos_x, pos_y);
		if (pos > m_text.size()) {
			return m_text.size();
		} else if (pos < 0) {
			return 0;
		}
		return pos;
	}
	return -1;
}

//----------------------------------
//PositionCursor
//----------------------------------

ui::PositionCursor::PositionCursor() {
	m_clock.start();
	//figure out how to load images as apparently it is buggy as hell
	/*try {
		m_texture = ci::gl::Texture(ci::loadImage("C:/Users/Robert/Desktop/CinderProject/resources/positioncursor.cur"));
		} catch (ci::app::ResourceLoadExc e) {
		ci::app::console() << e.what();
		}*/
}

ui::PositionCursor::~PositionCursor() {
	m_clock.stop();
}

void ui::PositionCursor::draw() {
	if (m_clock.getSeconds() - m_time == 1.0) {
		ci::gl::drawLine(m_position, ci::Vec2f(m_position.x, m_position.y + 10));
		m_time = m_clock.getSeconds();
	}
}