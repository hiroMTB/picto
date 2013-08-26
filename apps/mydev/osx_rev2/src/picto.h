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
    static float ICON_POSITION_RANDOMNESS;
    static float ICON_DISTANCE;
    
    
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
    void setPos(const ofPoint& p){ pos = p; }
    
    void setNewTarget(ofPoint p, int time=1000);
    void setParent(pictoChar * p){ parent = p; }
    
    ofPoint& getPos(){ return pos; }
    
    static float getSPEED(){ return SPEED; }
    static float getACCEL(){ return ACCEL; }
    static float getHoldTime(){ return HOLD_TIME; }
    static float getICON_DISTANCE(){ return ICON_DISTANCE; }
    static float getICON_POSITION_RANDOMNESS(){ return ICON_POSITION_RANDOMNESS; }
    
    static void  setSPEED(float f){ SPEED = f; }
    static void  setACCEL(float f){ ACCEL = f; }
    static void  setHoldTime(int i){ HOLD_TIME = i; }
    static void  setICON_DISTANCE(float f){ ICON_DISTANCE = f; }
    static void  setICON_POSITION_RANDOMNESS(float f){ ICON_POSITION_RANDOMNESS = f; }
    
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
    

public:

    struct Animation{
    public:
        enum AnimationType{
            none   = 0,
            random = 1,
            target = 2,
            end    = 4
        }type;

        long exeTime;       // system time
        
        bool done;
        
        ofPoint randPoint;
        float rw, rh;
        
        bool randomWalk;
        bool globalPos;
        
        Animation(AnimationType _type, long _exeTime, ofPoint _randPoint, float _rw, float _rh, bool _randomWalk, bool _globalPos=false):
        type(_type), exeTime(_exeTime), randPoint(_randPoint), rw(_rw), rh(_rh), randomWalk(_randomWalk), globalPos(_globalPos), done(false)
        {
        }
        
    };

    typedef pair<int, Animation> AnimationData;
    typedef map<int, Animation> AnimationCont;
    typedef AnimationCont::iterator AnimationContItr;
    AnimationCont animations;
    
    void updateAnimations();

    void setTargetAround(ofPoint p, float rw, float rh, bool randomWalk, bool globalPos);
    void setFinalTarget(bool randomWalk);

    void setRandomAnimation (int milliseconds, ofPoint p, float rw, float rh, bool _randomWalk, bool globalPos=false);
    void setFinalAnimation  (int milliseconds, bool randomWalk);
    void setEndAnimation    (int milliseconds, bool randomWalk);
    
    static void drawFontText(string s, int x, int y){
        
        float scale = FONT_SIZE/500.0;
        float fh = font.getLineHeight();

        glPushMatrix();
        glScalef(scale, scale, scale);
//        font.drawStringAsShapes(s, x, y+fh);
        font.drawString(s, x, y+fh);
        glPopMatrix();
    }
    
    
    static ofTrueTypeFont& getFont(){ return font; }
    static float getFontScale(){
        float scale = FONT_SIZE/500.0;
        return scale;
    }

//    static ofFbo fbo;
//    
//    void drawFbo(){
//        fbo.draw(0, 0);
//    };
};
