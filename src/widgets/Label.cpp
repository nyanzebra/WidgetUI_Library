#include "Widget.h"

ui::Label::Label(const ci::XmlTree& node) : Widget(node) {
	for (auto attr : node.getAttributes()) {
		if (attr.getName() == "font") {
			m_font = ci::Font(attr.getValue(), m_font_size);
		} else if (attr.getName() == "fontsize") {
			m_font_size = attr.getValue<float>();
		} else if (attr.getName() == "textcolor") {
			m_text_color = ci::Color::hex(attr.getValue<int>());
		} else if (attr.getName() == "backgroundcolor") {
			m_background_color = ci::Color::hex(attr.getValue<int>());
		}
	}
}

void ui::Label::render() {
	m_textbox = ci::TextBox();
	m_textbox.size(m_size);
	m_textbox.text(m_text).font(m_font);
	m_textbox.color(m_text_color).backgroundColor(m_background_color);
	m_textbox.alignment(m_alignment);
	m_texture = ci::gl::Texture(m_textbox.render());
}

