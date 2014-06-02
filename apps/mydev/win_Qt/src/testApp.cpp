#include "testApp.h"
#include "attractor.h"
#include "gpuPictoString.h"
#include "gpuPicto.h"

ofImage testApp::wc;
testApp * testApp::instance = NULL;
gpuPictoString * testApp::gps = NULL;

testApp::GlobalPrm testApp::gprm;

string testApp::pdfCapturePath = "";

ofEasyCam testApp::cam;

bool testApp::bCap = false;
bool testApp::bNeedCamUpdate = false;

void testApp::setup(){

//    ofSetLogLevel(OF_LOG_VERBOSE);

	cout << ofGetVersionInfo() << endl;
    
    //ofSetVerticalSync(true);
    //ofSetFrameRate(60);
    attractor::init();
    
    wc.loadImage("testPicture.jpg");

    gps = new gpuPictoString();

    cam.reset();
    //cam.setOrientation(ofQuaternion(180, ofVec3f(1,0,0)));

    loadDefaultSetting();
}

#include "pictoController.h"

void testApp::update(){

//    printf("cam::distance = %f", cam.getDistance());

    attractor::update();
    gps->update();
    
    if(bNeedCamUpdate){
        if(gprm.bWallMapMouseAdjust){
            cam.enableMouseInput();
        }else{
            cam.disableMouseInput();
        }
    }    
}

void testApp::draw(){
    int w = getW();
    int h = getH();

    if(bCap){
        ofBeginSaveScreenAsPDF(pdfCapturePath, false, false, ofRectangle(0,0,w,h));
    }
    ofBackground(0);
    if(gprm.bBlack){
        return;
    }

//    cam.begin();
//    ofRotate(180, 1, 0, 0);
//    ofTranslate(-w/2, -h/2);

    if(bCap){
        //ofBeginSaveScreenAsPDF(pdfCapturePath, false, false, ofRectangle(0,0,w,h));
        gps->drawForPdf();
        ofEndSaveScreenAsPDF();
        bCap = false;
    }else{        
        gps->draw();
    }
            
//    cam.end();
    
    if(gprm.bTestPicture){
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofSetColor(255, 255, 255, 150);
        float asp = wc.getWidth()/wc.getHeight();
        wc.draw(w/2, h/2, h*asp, h);
        ofSetRectMode(OF_RECTMODE_CORNER);
    }
}

void testApp::keyPressed(int key){
    ofToggleFullscreen();
}
void testApp::keyReleased(int key){}
void testApp::mouseMoved(int x, int y){

    //gps->offset.set((float)x/w, (float)y/h);
}

void testApp::mouseDragged(int x, int y, int button){}
void testApp::mousePressed(int x, int y, int button){}
void testApp::mouseReleased(int x, int y, int button){}
void testApp::windowResized(int _w, int _h){
    //cout << "testApp::windowResized, " << _w << ", " << _h << endl;
    cam.setDistance(cam.getImagePlaneDistance(ofRectangle(0, 0, _w, _h)), true);
    if(gps)gps->resize(_w, _h);
}
void testApp::gotMessage(ofMessage msg){}
void testApp::dragEvent(ofDragInfo dragInfo){}


void testApp::setFullscreen(bool b){
    //ofSetFullscreen(b);
    //user->setFullscreen(b);
}

void testApp::makeAnimation(){ gps->makeAnimation(); }

void testApp::clearAll(){ gps->clearAll(); }
void testApp::drawPreview(){ gps->drawPreview(); }


bool testApp::isNeedStartNextAnimation(){
    return gps->bShouldStartNext;
}

void testApp::finishStartNextAnimation(){
    gps->bShouldStartNext = false;
}


#include "pictoController.h"

string testApp::defaultSettingFileName = "defaultSetting.xml";

void testApp::loadDefaultSetting(){
    PrmData prm;

    ofxXmlSettings xml;

    bool ok = xml.loadFile(defaultSettingFileName);
    if(ok){
        if(xml.pushTag("default")){
            prm.message = xml.getValue("message", "??");
            prm.fontSize = xml.getValue("fontSize", 0.2);
            prm.lineHeight = xml.getValue("lineHeight",1.1 );
            prm.letterSpacing = xml.getValue("letterSpacing", 1.3);
            prm.iconSize = xml.getValue("iconSize", 0.04);
            prm.iconDensity = xml.getValue("iconDensity", 0.00002);
            prm.fontRandomness = xml.getValue("fontRandomness", 0.01);
            prm.speed = xml.getValue("speed", 20);
            prm.accel = xml.getValue("accel", 20);
            prm.vibration = xml.getValue("vibration", 0);
            prm.holdTime = xml.getValue("holdTime", 1000);
            gpuPictoString::prm = prm;

            gprm.bg.r = xml.getValue("backgroundColor_red", 0);
            gprm.bg.g = xml.getValue("backgroundColor_green", 0);
            gprm.bg.b = xml.getValue("backgroundColor_blue", 0);

            gprm.bTestPicture = xml.getValue("testPicture", 0);
            gprm.bShowInfo = xml.getValue("showInfo", 0);
            gprm.bBlack = xml.getValue("black", 0);
            gprm.bWallMapMouseAdjust = xml.getValue("wallMapMouseAdjust", 0);
            gprm.bDebugDraw = xml.getValue("debugDraw", 0);

            gprm.bAutoPlay = xml.getValue("autoPlay", 0);
            gprm.bLoop = xml.getValue("loop", 0);


            // synch GUI
            pictoController * c = pictoController::getInstance();
            c->setParameterFromPrmData(prm);
            c->setGlobalParam(gprm);
        }
    }

}

void testApp::saveDefaultSetting(){
    ofxXmlSettings xml;

    const PrmData &prm = gpuPictoString::prm;

    xml.addTag("default");
    xml.pushTag("default",0);{
        xml.addValue("message", prm.message);
        xml.addValue("fontSize", prm.fontSize);
        xml.addValue("lineHeight", prm.lineHeight);
        xml.addValue("letterSpacing", prm.letterSpacing);
        xml.addValue("iconSize", prm.iconSize);
        xml.addValue("iconDensity", prm.iconDensity);
        xml.addValue("iconSize", prm.iconSize);
        xml.addValue("fontRandomness", prm.fontRandomness);
        xml.addValue("speed", prm.speed);
        xml.addValue("accel", prm.accel);
        xml.addValue("vibration", prm.vibration);
        xml.addValue("holdTime", prm.holdTime);

        xml.addValue("backgroundColor_red", gprm.bg.r);
        xml.addValue("backgroundColor_green", gprm.bg.g);
        xml.addValue("backgroundColor_blue", gprm.bg.b);

        xml.addValue("testPicture", gprm.bTestPicture);
        xml.addValue("showInfo", gprm.bShowInfo);
        xml.addValue("black", gprm.bBlack);
        xml.addValue("wallMapMouseAdjust", gprm.bWallMapMouseAdjust);
        xml.addValue("debugDraw", gprm.bDebugDraw);
        xml.addValue("autoPlay", gprm.bAutoPlay);
        xml.addValue("loop", gprm.bLoop);
    }

    bool ok = xml.saveFile(defaultSettingFileName);
    if(ok) cout << "Save XML" << endl;
}

void testApp::exit(){
    saveDefaultSetting();
}
