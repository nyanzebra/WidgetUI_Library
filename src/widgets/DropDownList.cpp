#include "Widget.h"

ui::DropDownList::DropDownList() {
	m_display = new Button();
	m_box = new Label();
	m_is_displayed = false;
	init();
}

ui::DropDownList::DropDownList(const ci::XmlTree& node) : Widget(node) {
	m_display = new Button();
	m_box = new Label();
	m_is_displayed = false;
	init();

	for (ci::XmlTree::ConstIter option = node.begin(); option != node.end(); ++option) {
		add(option->getValue());
	}
}

ui::DropDownList::~DropDownList() {
	delete m_display;
	delete m_box;
	for (unsigned i = 0; i < m_options.size(); ++i) {
		delete m_options[i];
	}
	m_options.clear();
}

void ui::DropDownList::init() {
	m_display->setSize(this->getSize());
	m_display->setPosition(this->getPos());
	m_display->setText("show");
	m_display->setFontSize(DefaultFontSize);
	m_display->setAlignment(ci::TextBox::CENTER);
	m_display->setParent(this);
	m_display->render();

	m_box->setSize(ci::Vec2i(60, 20)); //allow for customization later
	m_box->setAlignment(ci::TextBox::LEFT);
	m_box->setFontSize(DefaultFontSize);
	m_box->setPosition(calculatePosition(0));
	m_box->setParent(this);
	m_box->render();
}

void ui::DropDownList::draw() {
	if (!m_display->isHidden() && m_display->getTexture()) {
		ci::gl::draw(m_display->getTexture(), m_display->getPos());
	}
	if (!m_box->isHidden() && m_box->getTexture()) {
		ci::gl::draw(m_box->getTexture(), m_box->getPos());
	}
	for (unsigned i = 0; i < m_options.size(); ++i) {
		if (!m_options[i]->isHidden() && m_options[i]->getTexture()) {
			ci::gl::draw(m_options[i]->getTexture(), m_options[i]->getPos());
		}
	}
}

void ui::DropDownList::add(const std::string& text) {
	ui::Button* b = new Button();
	b->setText(text);
	b->setSize(ci::Vec2i(60, 20)); //allow for customization later
	b->hide();
	b->setAlignment(ci::TextBox::LEFT);
	b->setFontSize(DefaultFontSize);
	auto f = [](Widget* ddl, std::string text, unsigned number) {
		DropDownList* d = (DropDownList*)ddl;
		d->setBoxText(text);
		d->setCurrentOption(number);
	};
	b->bindFunction(std::bind(f, std::placeholders::_1, text, m_options.size()));
	b->setParent(this);
	m_options.push_back(b);
}

void ui::DropDownList::remove(Button button) {
	for (unsigned i = 0; i < m_options.size(); ++i) {
		if (m_options[i]->getPos() == button.getPos()) {
			m_options.erase(m_options.begin() + i);
		}
	}
}

void ui::DropDownList::render() {
	m_box->render();
	for (unsigned i = 0; i < m_options.size(); ++i) {
		m_options[i]->render();
	}
}

void ui::DropDownList::keyDown(ci::app::KeyEvent& event) {
	if (m_state == SELECTED && m_options.size() > 0) {
		if (event.getCode() == ci::app::KeyEvent::KEY_DOWN) {
			if (m_current_option + 1 < m_options.size()) {
				m_options[++m_current_option]->execute(this);
			}
		} else if (event.getCode() == ci::app::KeyEvent::KEY_UP) {
			if (m_current_option - 1 >= 0) {
				if (m_current_option > 0) {
					--m_current_option;
				}
				m_options[m_current_option]->execute(this);
			}
		}
	}
}

void ui::DropDownList::mouseDown(ci::app::MouseEvent& event) {
	if (event.isLeftDown()) {
		if (withinWidget(event.getPos())) {
			m_top_list = 0;
			for (int i = m_top_list; i < 4; ++i) {
				m_options[i]->show();
				m_options[i]->setPosition(calculatePosition(i - m_top_list));
			}
			m_is_displayed = true;
		} else if (m_is_displayed) {
			for (unsigned i = m_top_list; i < m_top_list + 4; ++i) {
				m_options[i]->mouseDown(event);
				m_options[i]->hide();
			}
		} else {
			m_is_displayed = false;
		}
	}
	int change = (int)event.getWheelIncrement();
	if (change) {
		if ((m_top_list - change - 4) > 0) {
			m_top_list = m_top_list - change - 4;
		}
		if ((m_top_list + change + 4) < m_options.size()) {
			m_top_list = m_top_list + change + 4;
		}
		for (unsigned i = m_top_list; i < m_top_list + 4; ++i) {
			m_options[i]->setPosition(calculatePosition(i - m_top_list));
		}
	}
}
