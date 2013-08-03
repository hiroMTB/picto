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
    
    //        float fw = font.stringWidth( ofToString(c) );
    //        float fh = font.stringHeight(ofToString(c));
    vector<ofPolyline> outlines;

    ofTrueTypeFont font;
    
//    vector<ofTTFCharacter> chars;
//    vector<ofPolyline> polylines;
    
    vector<pictoChar*> pictoString;

    ofPoint offsetPos;
    
    vector<ofFbo*> fbos;
    vector< vector<ofPoint> > sameples;
    
    string inputText;
    
    
    
    static const float originalSvgIconSize;
    float iconSize;
    float fontSize;
    float overlapRate;
    
    void initAnimation(string t);
    bool bCap;
    void testAnimation();
    
    bool bDebugDraw;

};
