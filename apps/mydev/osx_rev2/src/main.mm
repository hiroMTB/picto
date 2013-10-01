//#include "ofMain.h"
//#include "testApp.h"
//#include "ofAppGlutWindow.h"

//========================================================================
//int main( ){
//
//    ofAppGlutWindow window;
//	ofSetupOpenGL(&window, 1280, 780, OF_WINDOW);			// <-------- setup the GL context
//
//	// this kicks off the running of my app
//	// can be OF_WINDOW or OF_FULLSCREEN
//	// pass in width and height too:
//    testApp::init();
//	ofRunApp( testApp::getInstance());
//
//}



#import <Cocoa/Cocoa.h>

int main(int argc, char *argv[])
{
    return NSApplicationMain(argc, (const char **) argv);
}