#include "Widget.h"

#include "../ui/UserInterface.h"

ui::Button::Button(const unsigned& page) : m_page(std::move(page)) {}

ui::Button::Button(const ci::XmlTree& node) : Label(node) {}

void ui::Button::mouseDown(ci::app::MouseEvent& event) {
	if (event.isLeftDown() && withinWidget(event.getPos())) {
		if (this->getParent()) {
			this->getParent()->setState(SELECTED);
			m_function(this->getParent());
		} else {
			m_function(this);
		}
	}
}

void ui::Button::bindFunction(std::function<void(Widget*)> function) {
	m_function = function;
}
