#include "UserInterface.h"

unsigned ui::UserInterface::m_current_page = 0; //init as beginning of pages

ui::UserInterface::UserInterface() {}

ui::UserInterface::UserInterface(const std::string& file) : m_xml(ci::XmlTree(file)) {}

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
		m_xml = ci::XmlTree(ci::loadFile(file));
	} catch (ci::XmlTree::Exception e) {
		std::cout << e.what();
	}
}

void ui::UserInterface::saveXML(const std::string& file) {
	try {
		m_xml.write(ci::writeFile(file));
	} catch (ci::XmlTree::Exception e) {
		std::cout << e.what();
	}
}

//factory method
ui::Widget* ui::UserInterface::constructWidget(const std::string& name, const ci::XmlTree& node) const {
	if (name == "label") {
		return new Label(node);
	} else if (name == "button") {
		return new Button(node);
	} else if (name == "ddl") {
		return new DropDownList(node);
	} else if (name == "textview") {
		return new TextView(node);
	} else if (name == "chat") {
		return new ChatWindow(node);
	} else if (name == "cursor") {
		return new Cursor(node);
	} else if (name == "filemap") {
		return new FileMap(node);
	} else if (name == "inputbox") {
		return new InputBox(node);
	}
}

void ui::UserInterface::generate() {
	if (!m_xml.isDocument()) {
		return;
	}

	ci::XmlTree ui = m_xml.getChild("ui");

	std::string tab = "tab";
	for (ci::XmlTree::Iter page = ui.begin(); page != ui.end(); ++page) {
		Page* p = new Page();

		for (ci::XmlTree::Iter widget = page->begin(); widget != page->end(); ++widget) {
			Widget* w = constructWidget(widget->getTag(), *widget);
			p->add(w);
		}
		if (page->getAttributeValue<std::string>("tab") == "true") {
			add(p, page->getAttributeValue<std::string>("name"));
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
		if (m_tabs[i]->getPos() == tab.getPos()) {
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
		m_pages[m_current_page]->update();
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
				m_tabs[i]->setPosition(ci::Vec2f(m_tabs[i - 1]->getSize().x * i, 0));
				m_tabs[i]->render();
			} else {
				m_tabs[i]->setPosition(ci::Vec2f(0, 0));
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
			m_tabs[i]->setSize(size);
		}
	}
}

bool ui::UserInterface::withinTabs(const ci::Vec2f& pos) const {
	float x = pos.x, y = pos.y;

	if (m_tabs.size() == 0) {
		return false;
	}

	float xbegin = m_tabs[0]->getPos().x, ybegin = m_tabs[0]->getPos().y;
	float xend = m_tabs[m_tabs.size() - 1]->getSize().x + m_tabs[m_tabs.size() - 1]->getPos().x;
	float yend = m_tabs[m_tabs.size() - 1]->getSize().y + m_tabs[m_tabs.size() - 1]->getPos().y;
	if (x >= xbegin && x <= xend + xbegin && y <= yend + ybegin && y >= ybegin) {
		return true;
	}
	return false;
}

void ui::UserInterface::currentPage(const unsigned& page) { //have to do bound checking!
	m_current_page = page;
}
