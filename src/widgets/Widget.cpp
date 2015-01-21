#include "Widget.h"

ui::Widget::Widget() {}

ui::Widget::Widget(const ci::XmlTree& node) {
	m_node = node;//this might allow for easier writebacks
	for (auto attr : node.getAttributes()) {
		if (attr.getName() == "alignment") {
			if (g_alignment_map.find(attr.getValue()) == g_alignment_map.end()) {
				m_alignment = ci::TextBox::LEFT;
			} else {
				m_alignment = g_alignment_map.at(attr.getValue());
			}
		} else if (attr.getName() == "parent") {

		} else if (attr.getName() == "visible") {
			if (attr.getValue() == "true") {
				m_is_hidden = false;
			} else if (attr.getValue() == "false") {
				m_is_hidden = true;
			} else {
				throw new ci::XmlTree::Exception;
			}
		} else if (attr.getName() == "text") {
			m_text = attr.getValue();
		} else if (attr.getName() == "x") {
			m_size.x = attr.getValue<float>();
		} else if (attr.getName() == "posx") {
			m_position.x = attr.getValue<float>();
		} else  if (attr.getName() == "y"){
			m_size.y = attr.getValue<float>();
		} else if (attr.getName() == "posy") {
			m_position.y = attr.getValue<float>();
		}
	}
}

ui::Widget::~Widget() { delete m_parent_widget; }

void ui::Widget::draw() {
	if (!m_is_hidden && m_texture) {
		ci::gl::draw(m_texture, m_position);
	}
}

bool ui::Widget::withinWidget(const ci::Vec2f& pos) {
	if (m_is_hidden) {
		return false;
	}
	float x = pos.x, y = pos.y;
	float xbegin = m_position.x, ybegin = m_position.y;
	float xend = m_size.x, yend = m_size.y;
	if (x >= xbegin && x <= xend + xbegin && y <= yend + ybegin && y >= ybegin) {
		return true;
	}
	return false;
}

void ui::Widget::update() {
	m_node.write(ci::writeFile(m_node.getPath()));
}

void ui::Widget::mouseUp(ci::app::MouseEvent& event) {
	//m_state = NORMAL;
}

void ui::Widget::mouseDown(ci::app::MouseEvent& event) {
	if (withinWidget(event.getPos())) {
		m_state = SELECTED;
	} else {
		m_state = NORMAL;
	}
}

void ui::Widget::keyDown(ci::app::KeyEvent& event) {
	if (event.getCode() == ci::app::KeyEvent::KEY_LALT) {
		m_is_draggable = !m_is_draggable;
	}
}

void ui::Widget::mouseDrag(ci::app::MouseEvent& event) {
	if (m_is_draggable && m_state == SELECTED) {
		m_position = event.getPos();
		if (m_node.hasAttribute("x")) {
			m_node.setAttribute("x", m_position.x);
		}
		if (m_node.hasAttribute("y")) {
			m_node.setAttribute("x", m_position.y);
		}
	}
}
