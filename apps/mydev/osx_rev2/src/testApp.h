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
    
    bool bDebugDraw;
    bool bShowInfo;
    bool bCap;
    bool bRealtime;
    
    string inputText;
    
    vector<pictoChar*> pictoString;

    ofPoint offsetPos;
    
    void initAnimation(string t);
    void testAnimation();
    

};
