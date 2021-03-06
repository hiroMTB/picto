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
bool testApp::bWallMapMouseAdjust = false;
bool testApp::bTestPicture  = false;
bool testApp::bNeedCamUpdate = true;
bool testApp::bLoop         = false;
bool testApp::bAutoPlay     = false;

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

    cam.reset();
    //cam.setOrientation(ofQuaternion(180, ofVec3f(1,0,0)));
    wc.loadImage("testPicture.jpg");
    
}


void testApp::update(){
    
//    printf("cam::distance = %f", cam.getDistance());
    
    attractor::update();
    gps->update();
    
    if(bNeedCamUpdate){
        if(bWallMapMouseAdjust){
            cam.enableMouseInput();
        }else{
            cam.disableMouseInput();
        }
    }
}

void testApp::draw(){
    
    ofBackground(0);
    if(bBlack){
        return;
    }
    
    cam.begin();
    ofRotate(180, 1, 0, 0);
    ofTranslate(-w/2, -h/2);

    
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

    
    if(bTestPicture){
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofSetColor(255, 255, 255, 150);
        float asp = wc.getWidth()/wc.getHeight();
        wc.draw(w/2, h/2, h*asp, h);
        ofSetRectMode(OF_RECTMODE_CORNER);
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
void testApp::windowResized(int _w, int _h){
    
    cam.setDistance(cam.getImagePlaneDistance(ofRectangle(0, 0, _w, _h)), true);
    
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

void testApp::setWallMapMouseAdjust(bool b){
    bWallMapMouseAdjust = b;
    bNeedCamUpdate = true;
}

void testApp::setTestPicture(bool b){
    bTestPicture = b;
}

void testApp::makeAnimation(){ gps->makeAnimation(); }

void testApp::clearAll(){ gps->clearAll(); }
void testApp::drawPreview(){ gps->drawPreview(); }


bool testApp::isNeedStartNextAnimation(){ return gps->bShouldStartNext; }
void testApp::finishStartNextAmimation(){ gps->bShouldStartNext = false; }

