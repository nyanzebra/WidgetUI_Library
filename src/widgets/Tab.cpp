#include "Widget.h"

#include "../ui/UserInterface.h"

ui::Tab::Tab(const unsigned& page) : Button(page) {}
ui::Tab::Tab(const unsigned& page, const std::string& name) : Button(page) {
	m_text = name;
}

void ui::Tab::mouseDown(ci::app::MouseEvent& event) {
	if (event.isLeftDown() && withinWidget(event.getPos())) {
		ui::UserInterface::currentPage(m_page);//sets page to current page
		setBackgroundColor(ci::Color(SelectedTabColor)); //allow for changing selected tab color
	} 
}

void ui::Tab::mouseDrag(ci::app::MouseEvent& event) {}

void ui::Tab::mouseUp(ci::app::MouseEvent& event) {}
