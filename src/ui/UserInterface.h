#pragma once

#include <vector>
#include <memory>

#include "../widgets/Widget.h"

#include "cinder/app/FileDropEvent.h"
#include "cinder/app/MouseEvent.h"
#include "cinder/app/KeyEvent.h"
#include "cinder/xml.h"

namespace ui {

	static ci::XmlTree g_xml_node = ci::XmlTree();

	class Page {
	public:
		Page();
		~Page();

		void render();
		void update(ci::XmlTree& node, const std::string& filename);
		void draw();
		void mouseDown(ci::app::MouseEvent& event);
		void mouseUp(ci::app::MouseEvent& event);
		void mouseDrag(ci::app::MouseEvent& event);
		void keyDown(ci::app::KeyEvent& event);

		void add(Widget* widget);
		void remove(Widget* widget);

		void setName(const std::string& name) { m_name = name; }

		const std::string& getName() const { return m_name; }
	private:
		std::string m_name;
		std::vector<Widget*> m_widgets;
	};

	class UserInterface {
	public:
		UserInterface();
		UserInterface(const std::string& file);
		~UserInterface();

		void loadXML(const std::string& file);
		void saveXML(const std::string& file);
		void generate(); //used to generate objects from an xml file
		void loadImages();

		void add(Page* page);
		void add(Page* page, const std::string name);
		void add(Tab* tab);

		void remove(const Tab& tab);
		void remove(const Page& page);

		void mouseDown(ci::app::MouseEvent& event);
		void mouseMove(ci::app::MouseEvent& event);
		void mouseUp(ci::app::MouseEvent& event);
		void mouseDrag(ci::app::MouseEvent& event);
		void mouseWheel(ci::app::MouseEvent& event);
		
		void keyDown(ci::app::KeyEvent& event);
		void keyUp(ci::app::KeyEvent& event);

		void fileDrop(ci::app::FileDropEvent& event);

		void render();
		void update();
		void draw();
		void init();

		const std::vector<Tab*>& getTabs() const { return m_tabs; }
		const std::vector<Page*>& getPages() const { return m_pages; }

		void resetTabColors();
		void tabInit();
		void setTabSizes(const ci::Vec2f& size);

		static void currentPage(const unsigned& page);

	private:
		bool withinTabs(const ci::Vec2f& pos) const;

		Widget* constructWidget(const std::string& name, const ci::XmlTree& node) const;

		std::string m_filename = "";

		std::unordered_map<std::string, ci::ImageSourceRef> m_map_images;

		std::vector<Page*> m_pages;
		std::vector<Tab*> m_tabs;
		static unsigned m_current_page;
	};

}
