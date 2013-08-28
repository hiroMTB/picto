#include "testApp.h"
#include "pictoChar.h"

boost::posix_time::ptime testApp::appStartTime = boost::posix_time::microsec_clock::local_time();

testApp * testApp::instance = NULL;

bool testApp::bCap          = false;
bool testApp::bRealtime     = false;
bool testApp::bShowInfo     = false;
bool testApp::bBlack        = false;
bool testApp::bDebugDraw    = false;

float testApp::w = 0;
float testApp::h = 0;

ofColor testApp::bg = ofColor(0,0,0);

void testApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
  	ofBackground(255);
	ofSetColor(255);
    ps = new pictoString();
}


void testApp::update(){
    ps->update();
}

void testApp::draw(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    if(bBlack){
        ofBackground(0,0,0);
        return;
    }
    
    ofBackground(bg);
    ps->draw();
    drawInfo();
    //capture();
}

void testApp::drawInfo(){
    if(bShowInfo){
        ofPushMatrix();{
            ofTranslate(0,0);
            ofSetColor(ofColor(255,255,255)-bg);
            ofFill();
            ofRect(0, 0, w, 30);
            ofSetColor(bg);
            int y = 23;
            ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()),20,y);
            ofDrawBitmapString("picto num: " + ofToString(picto::totalPicto), 200, y);
            
            ofDrawBitmapString("Frame num: " + ofToString(ofGetFrameNum()), 400, y);
        }ofPopMatrix();
    }
}

void testApp::capture(){
    if(bCap){
        ofImage image;
        image.grabScreen(0, 0, w, h);
        image.saveImage("shot.png");
        bCap = false;
    }
}

void testApp::keyPressed(int key){}
void testApp::keyReleased(int key){}
void testApp::mouseMoved(int x, int y){}
void testApp::mouseDragged(int x, int y, int button){}
void testApp::mousePressed(int x, int y, int button){}
void testApp::mouseReleased(int x, int y, int button){}
void testApp::windowResized(int _w, int _h){ w = _w; h = _h; }
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


#define USE_BOOST_CLOCK 1

long testApp::getNow(){

#ifdef USE_BOOST_CLOCK
    boost::posix_time::time_duration pnowd = boost::posix_time::microsec_clock::local_time() - appStartTime;
    return pnowd.total_milliseconds();
#else
    return ofGetElapsedTimeMillis();
#endif
}


