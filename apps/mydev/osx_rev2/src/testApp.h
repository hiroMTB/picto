#pragma once

#include "ofMain.h"
#include "ofxSvg.h"
#include "picto.h"
#include "pictoString.h"

class testApp : public ofBaseApp{

    static testApp * instance;
    
public:
    
    static testApp * getInstance(){ return instance; }
    static void init(){
        if(!instance){ instance = new testApp(); }
    }
    
    void setup();
    void update();
    void draw();
    void drawInfo();
    void capture();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    static bool bBlack;
    static bool bDebugDraw;
    static bool bShowInfo;
    static bool bCap;
    static bool bRealtime;
    static float w, h;
    static ofColor bg;
    
    static boost::posix_time::ptime appStartTime;    
    
    static void setBlack(bool b);
    static void setFullscreen(bool b);
    static void setShowInfo(bool b);
    static void setBackgroundColor(int r, int g, int b);
    static void setDebugDraw(bool b);

    static float getW(){ return w; }
    static float getH(){ return h; }
    static long getNow();
    static bool getDebugDraw(){ return bDebugDraw; }
    static const ofColor& getBackgroundColor(){ return bg; }

    
    
    pictoString * ps;
    void makeAnimation(){ ps->makeAnimation(); }
    void clearAll(){ ps->clearAll(); }
    void setPreviewText(string s){ ps->setText(s); }
    void drawPreview(){ ps->drawPreview(); }
};
