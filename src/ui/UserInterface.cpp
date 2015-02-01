#include "UserInterface.h"

unsigned ui::UserInterface::m_current_page = 0; //init as beginning of pages

ui::UserInterface::UserInterface() {}

ui::UserInterface::UserInterface(const std::string& file) {
	m_filename = file;
	g_xml_node = ci::XmlTree(ci::loadFile(file));
}

ui::UserInterface::~UserInterface() {
	for (unsigned i = 0; i < m_pages.size(); ++i) {
		delete m_pages[i];
	}
	for (unsigned i = 0; i < m_tabs.size(); ++i) {
		delete m_tabs[i];
	}
	m_pages.clear();
	m_tabs.clear();
}

void ui::UserInterface::loadXML(const std::string& file) {
	try {
		m_filename = file;
		g_xml_node = ci::XmlTree(ci::loadFile(file));
	} catch (ci::XmlTree::Exception e) {
		std::cout << e.what();
	}
}

void ui::UserInterface::saveXML(const std::string& file) {
	try {
		g_xml_node.write(ci::writeFile(file));
	} catch (ci::XmlTree::Exception e) {
		std::cout << e.what();
	}
}

//factory method
// need to add remaining classes
ui::Widget* ui::UserInterface::constructWidget(const std::string& name, const ci::XmlTree& node) const {
	if (name == "label") {
		return new Label(node);
	} else if (name == "button") {
		return new Button(node);
	} else if (name == "ddl") {
		return new DropDownList(node);
	} else if (name == "textview") {
		return new TextView(node);
	} else if (name == "cursor") {
		return new Cursor(node);
	} else if (name == "inputbox") {
		return new InputBox(node);
	} /*else if (name == "cw") {
		return new CustomWidget(node);
	}*/
}

void ui::UserInterface::loadImages() {
	ci::XmlTree images = g_xml_node.getChild("ui").getChild("images");

	for (ci::XmlTree::ConstIter image = images.begin(); image != images.end(); ++image) {
		if (image->hasAttribute("mapto") && image->getValue() != "") {
			m_map_images.insert({ image->getAttributeValue<std::string>("mapto"), ci::loadImage(image->getValue()) });
		}
	}
}

void ui::UserInterface::generate() {
	if (!g_xml_node.isDocument()) {
		return;
	}

	ci::XmlTree ui = g_xml_node.getChild("ui");

	for (ci::XmlTree::Iter page = ui.begin(); page != ui.end(); ++page) {
		Page* p = new Page();
		for (ci::XmlTree::Iter widget = page->begin(); widget != page->end(); ++widget) {
			Widget* w = constructWidget(widget->getTag(), *widget);
			p->add(w);
		}
		/*for (ci::XmlTree::Iter widget = page->begin(); widget != page->end(); ++widget) {
			if (widget->getTag() != "for"){
				Widget* w = constructWidget(widget->getTag(), *widget);
				p->add(w);
			} else {
				float posx, posy = 0;
				posx = widget->getAttributeValue<float>("posx");
				posy = widget->getAttributeValue<float>("posy");
				auto widgets = widget->getChildren();
				unsigned begin, end;
				begin = widget->getAttributeValue<unsigned>("begin");
				end = widget->getAttributeValue<unsigned>("end");
				for (unsigned i = begin; i < end; ++i) {
					Widget* w = constructWidget(widgets.front()->getTag(), *widgets.front());
					if (widget->hasAttribute("column")) {
						if (i % widget->getAttributeValue<unsigned>("column") == 0) {
							posy += widgets.front()->getAttributeValue<unsigned>("y");
							w->m_position.y = posy;
							p->add(w);
						}
					} else {
						p->add(w);
					}
					posx += widgets.front()->getAttributeValue<unsigned>("x");
				}
			}
		}*/
		if (page->hasAttribute("tab")) {
			if (page->getAttributeValue<std::string>("tab") == "true") {
				add(p, page->getAttributeValue<std::string>("name"));
			}
		} else {
			add(p);
		}
	}
}

void ui::UserInterface::add(Page* page) {
	m_pages.push_back(page);
}

void ui::UserInterface::add(Page* page, const std::string name) {
	m_pages.push_back(page);
	m_tabs.push_back(new Tab(m_pages.size() - 1, name));
}

void ui::UserInterface::add(Tab* tab) {
	m_tabs.push_back(tab);
}

void ui::UserInterface::remove(const Page& page) {
	for (unsigned i = 0; i < m_pages.size(); ++i) {
		if (m_pages[i]->getName() == page.getName()) {
			m_pages.erase(m_pages.begin() + i);
			if (m_tabs[i]) {
				m_tabs.erase(m_tabs.begin() + i);
			}
			break;
		}
	}
}

void ui::UserInterface::remove(const Tab& tab) {
	for (unsigned i = 0; i < m_tabs.size(); ++i) {
		if (m_tabs[i]->m_position == tab.m_position) {
			m_tabs.erase(m_tabs.begin() + i);
			break;
		}
	}
}

void ui::UserInterface::init() {
	m_current_page = 0;
}

void ui::UserInterface::mouseDown(ci::app::MouseEvent& event) {
	if (event.isRightDown()) {

	}

	if (m_pages[m_current_page]) {
		m_pages[m_current_page]->mouseDown(event);
	}

	if (withinTabs(event.getPos())) {
		resetTabColors();
	}

	for (unsigned i = 0; i < m_tabs.size(); ++i) {
		if (m_tabs[i]) {
			m_tabs[i]->mouseDown(event);
		}
	}
}

void ui::UserInterface::mouseUp(ci::app::MouseEvent& event) {
	if (m_pages[m_current_page]) {
		m_pages[m_current_page]->mouseUp(event);
	}
	for (unsigned i = 0; i < m_tabs.size(); ++i) {
		if (m_tabs[i]) {
			m_tabs[i]->mouseUp(event);
		}
	}
}

void ui::UserInterface::mouseDrag(ci::app::MouseEvent& event) {
	if (m_pages[m_current_page]) {
		m_pages[m_current_page]->mouseDrag(event);
	}
	for (unsigned i = 0; i < m_tabs.size(); ++i) {
		if (m_tabs[i]) {
			m_tabs[i]->mouseDrag(event);
		}
	}
}

void ui::UserInterface::keyDown(ci::app::KeyEvent& event) {
	if (m_pages[m_current_page]) {
		m_pages[m_current_page]->keyDown(event);
	}
}

void ui::UserInterface::render() {
	for (unsigned i = 0; i < m_tabs.size(); ++i) {
		if (m_tabs[i]) {
			m_tabs[i]->render();
		}
	}
	if (m_pages[m_current_page]) {
		m_pages[m_current_page]->render();
	}
}

void ui::UserInterface::update() {
	for (unsigned i = 0; i < m_tabs.size(); ++i) {
		if (m_tabs[i]) {
			m_tabs[i]->render();
		}
	}
	if (m_pages[m_current_page]) {
		m_pages[m_current_page]->update(g_xml_node, m_filename);
	}
}

void ui::UserInterface::draw() {
	for (unsigned i = 0; i < m_tabs.size(); ++i) {
		if (m_tabs[i]) {
			m_tabs[i]->draw();
		}
	}
	if (m_pages[m_current_page]) {
		m_pages[m_current_page]->draw();
	}
}

void ui::UserInterface::tabInit() {
	for (unsigned i = 0; i < m_tabs.size(); ++i) {
		if (m_tabs[i]) {
			m_tabs[i]->setAlignment(ci::TextBox::CENTER);
			if (i > 0) {
				m_tabs[i]->m_position = ci::Vec2f(m_tabs[i - 1]->m_size.x * i, 0);
				m_tabs[i]->render();
			} else {
				m_tabs[i]->m_position = ci::Vec2f(0, 0);
				m_tabs[i]->render();
			}
			//rest is defaulted
		}
	}
}

void ui::UserInterface::resetTabColors() {
	for (unsigned i = 0; i < m_tabs.size(); ++i) {
		if (m_tabs[i]) {
			m_tabs[i]->setBackgroundColor(ci::Color(WhiteColor));
		}
	}
}

void ui::UserInterface::setTabSizes(const ci::Vec2f& size) {
	for (unsigned i = 0; i < m_tabs.size(); ++i) {
		if (m_tabs[i]) {
			m_tabs[i]->m_size = size;
		}
	}
}

bool ui::UserInterface::withinTabs(const ci::Vec2f& pos) const {
	float x = pos.x, y = pos.y;

	if (m_tabs.size() == 0) {
		return false;
	}

	float xbegin = m_tabs[0]->m_position.x, ybegin = m_tabs[0]->m_position.y;
	float xend = m_tabs[m_tabs.size() - 1]->m_size.x + m_tabs[m_tabs.size() - 1]->m_position.x;
	float yend = m_tabs[m_tabs.size() - 1]->m_size.y + m_tabs[m_tabs.size() - 1]->m_position.y;
	if (x >= xbegin && x <= xend + xbegin && y <= yend + ybegin && y >= ybegin) {
		return true;
	}
	return false;
}

void ui::UserInterface::currentPage(const unsigned& page) { //have to do bound checking!
	m_current_page = page;
}
