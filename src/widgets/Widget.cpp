#include "Widget.h"

ui::Widget::Widget() {}

ui::Widget::Widget(const ci::XmlTree& node) {
	m_node = ci::XmlTree(node);//this might allow for easier writebacks
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
		} else if (attr.getName() == "y") {
			m_size.y = attr.getValue<float>();
		} else if (attr.getName() == "posy") {
			m_position.y = attr.getValue<float>();
		} else if (attr.getName() == "id") {

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

void ui::Widget::updateNode() {
	if (m_alignment == ci::TextBox::LEFT) {
		m_node.setAttribute("alignment", "left");
	} else if (m_alignment == ci::TextBox::CENTER) {
		m_node.setAttribute("alignment", "center");
	} else if (m_alignment == ci::TextBox::RIGHT) {
		m_node.setAttribute("alignment", "right");
	} else {
		m_node.setAttribute("alignment", "left");
	}
	if (m_is_hidden) {
		m_node.setAttribute("visible", "false");
	} else {
		m_node.setAttribute("visible", "true");
	}
	m_node.setAttribute("text", m_text);
	m_node.setAttribute("x", std::to_string(m_size.x));
	m_node.setAttribute("posx", std::to_string(m_position.x));
	m_node.setAttribute("y", std::to_string(m_size.y));
	m_node.setAttribute("posy", std::to_string(m_position.y));
}

void ui::Widget::update(ci::XmlTree& node, const std::string& filename) {
	if (node.hasChild("ui")) {
		updateNode();
		for (ci::XmlTree::Iter page = node.getChild("ui").begin(); page != node.getChild("ui").end(); ++page) {
			for (ci::XmlTree::Iter widget = page->begin(); widget != page->end(); ++widget) {
				if (widget->getAttribute("id") == m_node.getAttribute("id")) {
					*widget = m_node;
				}
			}
		}
		node.write(ci::writeFile(filename));
	}
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
	}
}
