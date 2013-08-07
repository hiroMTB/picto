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
    
};

class pictoChar{
    
private:
    static std::map<char, vector<ofPoint> > pointCharMap;
    
    char c;
    static ofTrueTypeFont font;
    static float   fontSizeDefualt;
    float         fontSize;

    static const int iconSizeOriginal = 128;       // png data size
    static float   iconSizeDefault;
    float         iconSize;                   // size for each charactor

    static float overlapRateDefault;
    float       overlapRate;

    static float stringAlphaDefault;
    float       stringAlpha;

    bool        bRandomWalk;
    
    ofVec3f    charPos;
    
public:

    static void initAlphabetFromFontdata();
    
    pictoChar(char _c, ofVec3f _charPos);
    
    typedef vector<picto*> PCON;
    typedef PCON::iterator PITR;
    PCON pcon;
    
    
    void update();
    
    void draw();
    void drawString();

    void drawTarget();
    void setTarget(vector<ofPoint> ps, bool randomWalk=true, bool grobalPos=true);
    
    int getInstanceNum(){ return pcon.size(); }

    ~pictoChar();
    void clearAll();
    
    void setRandomWalk(bool b){ bRandomWalk = b; }
    bool getRandomWalk(){ return bRandomWalk; }
    
    void setCharPos(ofVec3f v){ charPos = v; }
    ofVec3f getCharPos(){ return charPos; }
    
    void setAnimation(char c, int milliseconds, bool _randomWalk);
    void setAnimation(vector<ofPoint> ps, int milliseconds, bool _randomWalk, bool globalPos=true);
    
    
    
private:
    void setAnimationCallback(vector<ofPoint> ps, int milliseconds, bool _randomWalk, bool globalPos=true);

};


