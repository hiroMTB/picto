#include "testApp.h"
#include "pictoChar.h"
#include "attractor.h"
#include "gpuPictoString.h"
#include "gpuPicto.h"


testApp * testApp::instance = NULL;
gpuPictoString * testApp::gps = NULL;

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
//  	ofBackground(255);
//	ofSetColor(255);
    
//    pictoChar::init();
//    picto::init();
    attractor::init();
    
//    ps = new pictoString();
    
    gps = new gpuPictoString();
}


void testApp::update(){
    attractor::update();
    gps->update();
}

void testApp::draw(){
    ofBackground(0);
    
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    if(bBlack){
        return;
    }
    
    //ofBackground(bg);
    gps->draw();
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
            ofDrawBitmapString("picto num: " + ofToString(gpuPicto::totalPicto), 200, y);
            
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
void testApp::mouseMoved(int x, int y){

    //gps->offset.set((float)x/w, (float)y/h);
}

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

void testApp::makeAnimation(){ gps->makeAnimation(); }

void testApp::clearAll(){ gps->clearAll(); }
void testApp::setPreviewText(string s){ gps->text = s; }
void testApp::drawPreview(){ gps->drawPreview(); }


