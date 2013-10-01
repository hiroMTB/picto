#include "testApp.h"
#include "attractor.h"
#include "gpuPictoString.h"
#include "gpuPicto.h"

ofImage testApp::wc;


testApp * testApp::instance = NULL;
gpuPictoString * testApp::gps = NULL;

bool testApp::bCap          = false;
bool testApp::bRealtime     = false;
bool testApp::bShowInfo     = false;
bool testApp::bBlack        = false;
bool testApp::bDebugDraw    = false;

float testApp::w = 0;
float testApp::h = 0;
string testApp::pdfCapturePath = "";

ofColor testApp::bg = ofColor(0,0,0);

ofEasyCam testApp::cam;

void testApp::setup(){

	cout << ofGetVersionInfo() << endl;
    
    ofSetVerticalSync(true);
	ofSetFrameRate(60);
    attractor::init();
    
    gps = new gpuPictoString();
    cam.setOrientation(ofQuaternion(180, ofVec3f(1,0,0)));
    //cam.setPosition(100, 0, -cam.getDistance());

//    cam.setTranslationKey(OF_KEY_CTRL);
    wc.loadImage("theater3.jpg");
}


void testApp::update(){
    
//    printf("cam::distance = %f", cam.getDistance());
    
    attractor::update();
    gps->update();
}

void testApp::draw(){
    
    ofBackground(0);
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(255);
    
    float asp = wc.getWidth()/wc.getHeight();
    wc.draw(w/2, h/2, 720*asp, 720);
    ofSetRectMode(OF_RECTMODE_CORNER);

    cam.begin();
    ofTranslate(-w/2, -h/2);

    if(bBlack){
        return;
    }
    
    if(bCap){
        ofBeginSaveScreenAsPDF(pdfCapturePath, false, false);
        ofBackground(testApp::bg);
        gps->drawForPdf();
        
    }else{
        
        gps->draw();
    }
        
    if(bCap){
        ofEndSaveScreenAsPDF();
        bCap = false;
    }
    
    cam.end();
    
}




void testApp::keyPressed(int key){}
void testApp::keyReleased(int key){}
void testApp::mouseMoved(int x, int y){

    //gps->offset.set((float)x/w, (float)y/h);
}

void testApp::mouseDragged(int x, int y, int button){}
void testApp::mousePressed(int x, int y, int button){}
void testApp::mouseReleased(int x, int y, int button){}
void testApp::windowResized(int _w, int _h){
}
void testApp::gotMessage(ofMessage msg){}
void testApp::dragEvent(ofDragInfo dragInfo){}

void testApp::setBlack(bool b){
    bBlack = b;
}

void testApp::setFullscreen(bool b){
    ofSetFullscreen(b);
}

void testApp::setBackgroundColor(int r, int g, int b){
    bg.set(r,g,b);
}

void testApp::setDebugDraw(bool b){
    bDebugDraw = b;
}

void testApp::setShowInfo(bool b){
    bShowInfo = b;
}

void testApp::makeAnimation(){ gps->makeAnimation(); }

void testApp::clearAll(){ gps->clearAll(); }
void testApp::drawPreview(){ gps->drawPreview(); }


