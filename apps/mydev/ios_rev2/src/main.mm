#include "ofMain.h"
#include "testApp.h"

int main(){

    ofSetupOpenGL(1024, 768, OF_FULLSCREEN);        // iPad 2
	//ofSetupOpenGL(640,960, OF_FULLSCREEN);			// iPhone 4S

	ofRunApp(new testApp);
}
