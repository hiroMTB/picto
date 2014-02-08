#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

int main( ){

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1280, 780, OF_WINDOW);			// <-------- setup the GL context

    testApp::init();
	ofRunApp( testApp::getInstance());

}