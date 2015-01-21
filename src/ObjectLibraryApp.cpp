#include <vector>

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"

#include "widgets/Widget.h"

#include "ui/UserInterface.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace ui;

class ObjectLibraryApp : public AppNative {
public:
	void prepareSettings(Settings* settings);
	void setup();
	void mouseDown(MouseEvent event);
	void mouseUp(MouseEvent event);
	void mouseDrag(MouseEvent event);
	void keyDown(KeyEvent event);
	void update();
	void draw();
	void render();
private:
	UserInterface* m_ui;
};

void ObjectLibraryApp::prepareSettings(Settings* settings) {
	settings->setTitle("Object Library");
	settings->setWindowSize(800, 600);
}

void ObjectLibraryApp::setup() {
	//make a ui, pages, and if wanted tabs
	m_ui = new UserInterface();
	m_ui->loadXML("C:/Users/Robert/Desktop/CinderProject/src/xml/ui.xml");// figure out relative paths
	m_ui->generate();
	m_ui->setTabSizes(Vec2i(100, 60));
	m_ui->tabInit();//sets up rest of default tabs for you
	m_ui->init();//init with starting page
	//finish setup and rendering
	m_ui->render();
}

void ObjectLibraryApp::mouseDown(MouseEvent event) {
	m_ui->mouseDown(event);
}

void ObjectLibraryApp::mouseUp(MouseEvent event) {
	m_ui->mouseUp(event);
}

void ObjectLibraryApp::mouseDrag(MouseEvent event) {
	m_ui->mouseDrag(event);
}

void ObjectLibraryApp::keyDown(KeyEvent event) {
	m_ui->keyDown(event);
}

void ObjectLibraryApp::update() {
	m_ui->render();
}

void ObjectLibraryApp::draw() {
	// clear out the window with grey
	gl::clear(Color(GreyColor), true);

	m_ui->draw();
}

void ObjectLibraryApp::render() {
	m_ui->render();
}

CINDER_APP_NATIVE(ObjectLibraryApp, RendererGl)
