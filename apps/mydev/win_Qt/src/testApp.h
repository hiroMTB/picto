#pragma once

#include "ofMain.h"
#include "ofxSvg.h"
#include "ofxQtGLWidget.h"
#include "ofxXmlSettings.h"

class gpuPictoString;

class testApp : public ofBaseApp{

    static testApp * instance;
    
public:
    struct GlobalPrm{
        bool bBlack;
        bool bDebugDraw;
        bool bShowInfo;
        //static bool bRealtime;
        bool bTestPicture;
        bool bWallMapMouseAdjust;
        bool bAutoPlay;
        bool bLoop;

        ofColor bg;
    };

    static GlobalPrm gprm;


    static bool bNeedCamUpdate;
    static bool bCap;

    static testApp * getInstance(){ return instance; }
    static void init(){
        if(!instance){ instance = new testApp(); }
    }
    
    void setup();
    void update();
    void draw();
    void drawInfo();
    void exit();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    static void setFullscreen(bool b);

    float getW(){ return user!=NULL ? user->getWidth() : -12345.0; }
    float getH(){ return user!=NULL ? user->getHeight() : -12345.0; }

    static bool getDebugDraw(){ return gprm.bDebugDraw; }
    static const ofColor& getBackgroundColor(){ return gprm.bg; }

    static gpuPictoString * gps;

    void makeAnimation();
    void clearAll();
    void drawPreview();
    
    static string pdfCapturePath;
    
    static ofEasyCam cam;

    static ofImage wc;

    static bool isNeedStartNextAnimation();
    static void finishStartNextAnimation();


    void registerUserPointer(ofxQtGLWidget * p){ user = p; }



private:
    ofxQtGLWidget * user;

    void loadDefaultSetting();
    void saveDefaultSetting();
    static string defaultSettingFileName;


};
