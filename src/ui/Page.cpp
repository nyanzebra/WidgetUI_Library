#include "UserInterface.h"

ui::Page::Page() {}

ui::Page::~Page() {
	for (unsigned i = 0; i < m_widgets.size(); ++i) {
		delete m_widgets[i];
	}
	m_widgets.clear();
}

void ui::Page::render() {
	for (unsigned i = 0; i < m_widgets.size(); ++i) {
		if (m_widgets[i]) {
			m_widgets[i]->render();
		}
	}
}

void ui::Page::update() { //to widgets add a bool for if it has changed... if so they get updated here
	render();
}

void ui::Page::draw() {
	for (unsigned i = 0; i < m_widgets.size(); ++i) {
		if (m_widgets[i]) {
			m_widgets[i]->draw();
		}
	}
}

void ui::Page::mouseDown(ci::app::MouseEvent& event) {
	for (unsigned i = 0; i < m_widgets.size(); ++i) {
		if (m_widgets[i]) {
			m_widgets[i]->mouseDown(event);
		}
	}
}

void ui::Page::mouseDrag(ci::app::MouseEvent& event) {
	for (unsigned i = 0; i < m_widgets.size(); ++i) {
		if (m_widgets[i]) {
			m_widgets[i]->mouseDrag(event);
		}
	}
}

void ui::Page::mouseUp(ci::app::MouseEvent& event) {
	for (unsigned i = 0; i < m_widgets.size(); ++i) {
		if (m_widgets[i]) {
			m_widgets[i]->mouseUp(event);
		}
	}
}

void ui::Page::keyDown(ci::app::KeyEvent& event) {
	for (unsigned i = 0; i < m_widgets.size(); ++i) {
		if (m_widgets[i]) {
			m_widgets[i]->keyDown(event);
		}
	}
}

void ui::Page::add(Widget* widget) {
	m_widgets.push_back(widget);
}

void ui::Page::remove(Widget* widget) {
	for (unsigned i = 0; i < m_widgets.size(); ++i) {
		if (m_widgets[i]->getPos() == widget->getPos() || m_widgets[i] == widget) {
			m_widgets.erase(m_widgets.begin() + i);
			break;
		}
	}
}