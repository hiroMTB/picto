//
//  picto.h
//  pictoTest_rev01
//
//  Created by matobahiroshi on 13/07/03.
//
//

#pragma once
#include "ofMain.h"
#include "ofxSvg.h"
#include "ofxTweener.h"
#include <boost/asio.hpp>

//#define USE_SVG

#ifdef USE_SVG
typedef ofxSVG pImg;
#else
typedef ofImage pImg;
#endif

class pictoChar;

class picto{
  
public:
    static const int colorTypeNum = 5;
    static int totalPicto;
private:
    static int iconSizeOriginal;
    static int imgTypeNum;
    static int  HOLD_TIME;
    static float SPEED, ACCEL;
    static pImg * imgs;
    static const ofColor colors[colorTypeNum];

    int fixCount;
    int colorType, imgType;
    float scale, topSpeed, minSpeed, K, alpha;

    pictoChar * parent;
    ofVec2f pos, target, newTarget, acc, vel, dir;
    ofVec2f sep, ali, coh;
    
public:
    static void init();
    static void loadImgFromSeparateFile();
    static void loadImgFromAllstar(int row, int col, int num);
    
    picto();
    ~picto();
    
    void setRandom();
    void update();
    void targetSineMove();

    ofVec2f springSim();
    ofVec2f sameAccSim();
    ofVec2f gravitySim();
    void speedControl();
    ofVec2f separate(vector<picto*> friends, float distance);
    ofVec2f align(vector<picto*> friends, float distance);
    ofVec2f cohesion(vector<picto*> friends, float distance);
    void borders();
    
    void draw();
    void drawTarget();
    
    static int getIconSizeOriginal(){ return iconSizeOriginal; }
    static float getSPEED()     { return SPEED; }
    static float getACCEL()     { return ACCEL; }
    static float getHoldTime()  { return HOLD_TIME; }
    ofVec2f& getPos()         { return pos; }
    ofVec2f& getVel()         { return vel; }
    ofVec2f& getAcc()         { return acc; }
    
    static void  setSPEED(float f)  { SPEED = f; }
    static void  setACCEL(float f)  { ACCEL = f; }
    static void  setHoldTime(int i) { HOLD_TIME = i; }

    void setScale(float s)       { scale = s; }
    void setPos(const ofVec2f& p){ pos = p; }
    void setAcc(const ofVec2f& p){ acc = p; }
    void setVel(const ofVec2f& p){ vel = p; }
    
    void setColorType(int type)  { colorType = type%colorTypeNum; }
    void setImgType(int i)       { imgType = i; }
    void setNewTarget(ofVec2f p, int time=1000);
    void setParent(pictoChar * p){ parent = p; }

};
