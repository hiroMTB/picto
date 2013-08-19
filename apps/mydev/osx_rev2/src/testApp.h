#pragma once

#include "ofMain.h"
#include "ofxSvg.h"

#include "picto.h"

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
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    

    bool bShowInfo;
    bool bCap;
    bool bRealtime;
    
    string inputText;
    
    vector<pictoChar*> pictoString;

    ofPoint offsetPos;
    
    void makeAnimation(string s);
    void clearAll();
    void clearFromPictoString(pictoChar * c);



    // screen settings
    static bool bBlack;
    static bool bDebugDraw;
    static ofColor bg;
    
    static void setBlack(bool b);
    static void setFullscreen(bool b);
    static void setBackgroundColor(int r, int g, int b);
    static const ofColor& getBackgroundColor(){ return bg; }
    static void setDebugDraw(bool b);
};
