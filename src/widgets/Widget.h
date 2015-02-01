#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/app/MouseEvent.h"
#include "cinder/app/KeyEvent.h"
#include "cinder/Text.h"
#include "cinder/Timer.h"
#include "cinder/xml.h"
#include "cinder/app/App.h"

namespace ui {

#define DefaultFont "Times New Roman"
#define DefaultFontSize 20.0
#define FontLowerBound 6.0
#define FontUpperBound 36.0
#define BlackColor 0.0f, 0.0f, 0.0f
#define GreyColor 0.6f, 0.6f, 0.6f
#define WhiteColor 1.0f, 1.0f, 1.0f
#define DefaultSize 60, 20
#define ZeroSize 0,0
#define DefaultMaxCharPerLine 20

	//image suffixes
#define Prefix "../image/"
#define SuffixNormal "_normal.jpg"
#define SuffixSelected "_selected.jpg"
#define SuffixHighlighted "_highlighted.jpg"

	static std::unordered_map<double, int> g_font_point_map =
	{
		{ 6.0, 8 },
		{ 7.0, 9 },
		{ 7.5, 10 },
		{ 8.0, 11 },
		{ 9.0, 12 },
		{ 10.0, 13 },
		{ 10.5, 14 },
		{ 11.0, 15 },
		{ 12.0, 16 },
		{ 13.0, 17 },
		{ 13.5, 18 },
		{ 14.0, 19 },
		{ 14.5, 20 },
		{ 15.0, 21 },
		{ 16.0, 22 },
		{ 17.0, 23 },
		{ 18.0, 24 },
		{ 20.0, 26 },
		{ 22.0, 29 },
		{ 24.0, 32 },
		{ 26.0, 35 },
		{ 27.0, 36 },
		{ 28.0, 37 },
		{ 29.0, 38 },
		{ 30.0, 40 },
		{ 32.0, 42 },
		{ 34.0, 45 },
		{ 36.0, 48 },
	};

	static std::unordered_map<std::string, ci::TextBox::Alignment> g_alignment_map =
	{
		{ "left", ci::TextBox::LEFT },
		{ "center", ci::TextBox::CENTER },
		{ "right", ci::TextBox::RIGHT }
	};

	enum k_state {
		NORMAL,
		HIGHLIGHT,
		SELECTED,
	};

	static std::unordered_map<k_state, std::string> g_image_map = {
		{ NORMAL, SuffixNormal },
		{ HIGHLIGHT, SuffixHighlighted },
		{ SELECTED, SuffixSelected },
	};

	//update to unique pointers
	class Widget {
	public:
		Widget();
		Widget(const ci::XmlTree& node);
		virtual ~Widget();

		virtual void draw();

		void setParent(Widget* widget) { m_parent_widget = widget; }
		void setText(const std::string& text) { m_text = text; }
		void setAlignment(const ci::TextBox::Alignment& alignment) { m_alignment = alignment; }
		void setState(const k_state& state) { m_state = state; }

		virtual void mouseDown(ci::app::MouseEvent& event);
		virtual void mouseUp(ci::app::MouseEvent& event);
		virtual void mouseDrag(ci::app::MouseEvent& event);
		virtual void mouseMove(ci::app::MouseEvent& event) {}
		virtual void mouseWheel(ci::app::MouseEvent& event) {}

		virtual void keyDown(ci::app::KeyEvent& event);
		virtual void keyUp(ci::app::KeyEvent& event) {}

		virtual void render() = 0;
		virtual void update(ci::XmlTree& node, const std::string& filename);

		void updateNode();

		void hide() { m_is_hidden = true; }
		void show() { m_is_hidden = false; }

		const bool& isHidden() const { return m_is_hidden; }
		const unsigned& getID() const { return m_id; }
		const k_state& getState() const { return m_state; }
		const std::string& getText() const { return m_text; }
		Widget* getParent() const { return m_parent_widget; }
		const ci::TextBox::Alignment getAlignment() const { return m_alignment; }
		const ci::gl::Texture& getTexture() const { return m_texture; }

		ci::Vec2f m_size = ci::Vec2f::zero();
		ci::Vec2f m_position = ci::Vec2f::zero();
	protected:
		bool withinWidget(const ci::Vec2f& pos);
		bool m_is_hidden = false;
		ci::TextBox::Alignment m_alignment = ci::TextBox::LEFT;
		Widget* m_parent_widget = nullptr;
		std::string m_text = "";

		ci::gl::Texture m_texture;

		ci::XmlTree m_node;

		k_state m_state = NORMAL;
	private:
		unsigned m_id;
		bool m_is_draggable = false;
	};

	class Cursor : public Widget {
	public:
		Cursor();
		Cursor(const ci::XmlTree& node);
		~Cursor();

		void render() {}
	};

	class PositionCursor : public Widget {
	public:
		PositionCursor();
		~PositionCursor();

		void render() {}
		void draw();
	private:
		double m_time = 0.0;
		ci::Timer m_clock;
	};

	class Label : public Widget {
	public:
		Label() = default;
		Label(const ci::XmlTree& node);

		void render();

		void setTextColor(const ci::Color& color) { m_text_color = color; }
		void setBackgroundColor(const ci::Color& color){ m_background_color = color; }
		void setFontSize(const float& size) { m_font_size = size; }
		void setFont(const ci::Font& font) { m_font = font; }

		const float& getFontSize() const { return m_font_size; }
		const ci::Font getFont() const { return m_font; }
		const ci::Color getTextColor() const { return m_text_color; }
		const ci::Color getBackgroundColor() const { return m_background_color; }

	protected:
		float m_font_size = DefaultFontSize;
		ci::Color m_text_color = ci::Color(BlackColor);
		ci::Color m_background_color = ci::Color(WhiteColor);
		ci::Font m_font = ci::Font(DefaultFont, m_font_size);

		ci::TextBox m_textbox;
	};

#define SelectedTabColor 0, 1, 0

	class Button : public Label {
	public: //allow for redirects to different pages similar to tabs
		Button() = default;
		Button(const ci::XmlTree& node);
		Button(const unsigned& page);

		void mouseDown(ci::app::MouseEvent& event);

		void bindFunction(std::function<void(Widget*)> function);

		void execute(Widget* widget) { m_function(widget); }

	protected:
		unsigned m_page;

	private:
		std::function<void(Widget*)> m_function; //bind ui element and function
	};

	class Tab : public Button {
	public:
		Tab(const unsigned& page);
		Tab(const unsigned& page, const std::string& name);

		void mouseDown(ci::app::MouseEvent& event);
		void mouseDrag(ci::app::MouseEvent& event);
		void mouseUp(ci::app::MouseEvent& event);
	};

#define DefaultCharSize 6
#define DefaultStringSize 7

	class InputBox : public Label {
	public:
		InputBox() = default;
		InputBox(const ci::XmlTree& node);

		void mouseDown(ci::app::MouseEvent& event);

		void keyDown(ci::app::KeyEvent& event);

		void render();
		void draw();
	protected:
		unsigned m_current_char = 0;

		PositionCursor* m_text_cursor = new PositionCursor();

		virtual const int calculateCharPosition(const ci::Vec2f& position);

		virtual const bool isProperChar(const int& i);
	private:
		bool m_is_writable = false;
	};

	class DropDownList : public Widget {
	public:
		DropDownList();
		DropDownList(const ci::XmlTree& node);
		~DropDownList();

		void init();

		void add(const std::string& text);
		void remove(Button button);

		void setBoxText(const std::string& text) { m_box->setText(text); }
		void setCurrentOption(const unsigned& option) { m_current_option = option; }

		void draw();
		void render();
		void keyDown(ci::app::KeyEvent& event);
		void mouseDown(ci::app::MouseEvent& event);
	private:
		const ci::Vec2f calculatePosition(const int& mult) {
			return ci::Vec2f(this->m_position.x - 60, this->m_position.y + (mult * 20));
		}

		unsigned m_current_option = 0;
		std::vector<Button*> m_options;
		unsigned m_top_list = 0;
		Button* m_display;
		Label* m_box;
		bool m_is_displayed = false;
	};

	class TextView : public InputBox {
	public:
		TextView(const ci::XmlTree& node);

		void render();
		void mouseDown(ci::app::MouseEvent& event);
		void keyDown(ci::app::KeyEvent& event);

		void setFilename(const std::string& filename) { m_filename = filename; }

		const std::string& getFilename() const { return m_filename; }

		void writeContents() const {}
		void loadContents();

	private:
		const int repatternText();

		const unsigned calculateCharPosition();

		const bool isProperChar(const int& i);

		std::string m_filename = "";
	};

	/*class CustomWidget : public Widget {
	public:
		class WidgetArgs {
		public:
			WidgetArgs ();
			~WidgetArgs ();

		private:
			std::list<Widget*> m_args;
		};

		CustomWidget() = default;
		CustomWidget(const ci::XmlTree& node);

		void mouseDown(ci::app::MouseEvent& event);
		void mouseWheel(ci::app::MouseEvent& event);
		void keyDown(ci::app::MouseEvent& event);
		void render();
		
		void bindFunction(std::function<void(WidgetArgs)> function);
	private:
		std::function<void(WidgetArgs)> m_function;
	};*/

}
