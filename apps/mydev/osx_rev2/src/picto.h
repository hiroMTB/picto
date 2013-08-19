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
  
private:
    static int imgTypeNum;
    static pImg * imgs;
    
    pictoChar * parent;
    
    ofVec3f pos, target, newTarget;
    ofVec3f acc, vel, dir;

    ofQuaternion quat;

    float angle;
    float scale;
    float topSpeed, minSpeed;
    float spLength;
    float K;
    
    int colorType;
    float alpha;
    int imgType;

    static float SPEED;
    static float ACCEL;
    static int  HOLD_TIME;
    
public:
    static const int colorTypeNum = 5;
    static const ofColor colors[colorTypeNum];
    
    
    static void init();
    static void loadImgFromSeparateFile();
    static void loadImgFromAllstar(int row, int col, int num);
    
    picto();
    ~picto();
    
    void setRandom();

    void update();
    
    void targetSineMove();
    
    void springSim();
    void sameAccSim();
    void gravitySim();
    void speedControl();
    
    void draw();
    void drawTarget();
    
    void setPos(ofPoint p){ pos = p; }
    void setScale(float s){ scale = s; }
    void setAngle(float a){ angle = a; }
    void setColorType(int type){ colorType = type%colorTypeNum; }
    void setImgType(int i){ imgType = i; }
    
    void setNewTarget(ofPoint p, int time=1000);
    void setParent(pictoChar * p){ parent = p; }
    
    static float getSPEED(){ return SPEED; }
    static float getACCEL(){ return ACCEL; }
    static float getHoldTime(){ return HOLD_TIME; }
    static void  setSPEED(float f){ SPEED = f; }
    static void  setACCEL(float f){ ACCEL = f; }
    static void  setHoldTime(int i){ HOLD_TIME = i; }
    
};

class pictoChar{
    
private:
    static std::map<char, vector<ofPoint> > pointCharMap;

    vector<ofPoint> finalTarget;

    char c;
    static ofTrueTypeFont font;
    static float   FONT_SIZE;
    float         fontSize;

    static const int iconSizeOriginal = 128;       // png data size
    static float   ICON_SIZE;
    float         iconSize;                   // size for each charactor

    static float overlapRateDefault;
    float       overlapRate;

    static float stringAlphaDefault;
    float       stringAlpha;

    bool        bRandomWalk;
    bool        bClearance;
    int         clearanceCounter;
    ofVec3f    charPos;
    
    float width, height;
    
public:

    static void initAlphabetFromFontdata();
    static vector<ofPoint>  makeCharacterPointData(char c, float res);
    
    pictoChar(char _c, ofVec3f _charPos);
    
    typedef vector<picto*> PCON;
    typedef PCON::iterator PITR;
    PCON pcon;
    
    
    void update();
    
    void draw();
    void drawString();

    void drawTarget();

    int getInstanceNum(){ return pcon.size(); }

    ~pictoChar();
    void clearAnimation(int waitTime);
    bool clearanceCheck();
    void destroy();
    
    void setRandomWalk(bool b){ bRandomWalk = b; }
    bool getRandomWalk(){ return bRandomWalk; }
    bool getClearance(){ return bClearance; }
    
    void setCharPos(ofVec3f v){ charPos = v; }
    ofVec3f getCharPos(){ return charPos; }
    
    static float getFONT_SIZE(){ return FONT_SIZE; }
    static float getICON_SIZE(){ return ICON_SIZE; }
    static void  setFONT_SIZE(float f){ FONT_SIZE = f; }
    static void  setICON_SIZE(float f){ ICON_SIZE = f; }
    
    float getWidth(){ return width; }
    float getHeight(){ return height; }
    
    //    void setTarget(vector<ofPoint> ps, bool randomWalk=true, bool grobalPos=true, bool forceExecute=false);
    void setTargetAround(ofPoint p, float rw, float rh, bool randomWalk, bool grobalPos, bool forceExecute);
    void setRandomAnimation(ofPoint p, float rw, float rh, int milliseconds, bool _randomWalk, bool grobalPos=true, bool forceExecute=false);

    void setFinalTarget(bool _randomWalk, bool globalPos, bool forceExecute);
    void setFinalAnimation(int milliseconds, bool _randomWalk, bool globalPos=true, bool forceExecute=false);

private:
    void setRandomAnimationCallback(ofPoint p, float rw, float rh, int milliseconds, bool _randomWalk, bool globalPos=true, bool forceExecute=false);
    void setFinalAnimationCallback(int milliseconds, bool _randomWalk, bool globalPos=true, bool forceExecute=false);

};
