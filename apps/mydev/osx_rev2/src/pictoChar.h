//
//  pictoChar.h
//  osx_picto_rev02
//
//  Created by matobahiroshi on 2013/08/27.
//
//

#pragma once

#include "picto.h"

class pictoString;

class pictoChar{
    
public:
    typedef vector<picto*> PCON;
    typedef PCON::iterator PITR;
    
    pictoChar(char _c, ofVec2f _charPos, pictoString * _paarent);
    ~pictoChar();

private:
    PCON pcon;
    
    pictoString * parent;
    
    static float stringAlphaDefault;
    static float FONT_SIZE, ICON_SIZE, FONT_RANDOMNESS, ICON_DISTANCE, LINE_HEIGHT, LETTER_SPACING;
    
    static std::map<char, vector<ofVec2f> > pointCharMap;
    static ofTrueTypeFont font;
    
    vector<ofVec2f> finalTarget;
    
    char c;
    bool bRandomWalk, bClearance, bFixed;

    int clearanceCounter;

    float fontSize, stringAlpha;
    float width, height;
    ofVec2f charPos;
    
    ofFbo fbo;
    
public:
 
    
    static void init();
    static void drawFontText(string s, int x, int y);
    
    vector<ofVec2f>  makeCharacterPointData(char c, float res);
    
    void update();
    void draw();
    void drawString();
    void drawTarget();
    
    
    bool clearanceCheck();
    
    static float getFontScale(){ return FONT_SIZE/500.0; }
    static float getFONT_SIZE()         { return FONT_SIZE; }
    static float getICON_SIZE()         { return ICON_SIZE; }
    static float getICON_DISTANCE()     { return ICON_DISTANCE; }
    static float getFONT_RANDOMNESS()   { return FONT_RANDOMNESS; }
    static float getLETTER_SPACING()    { return LETTER_SPACING; }
    static float getLINE_HEIGHT()       { return LINE_HEIGHT; }
    static ofTrueTypeFont& getFont(){ return font; }
    
    bool getRandomWalk()    { return bRandomWalk; }
    bool getClearance()     { return bClearance; }
    bool getFixed()         { return bFixed; }
    int getInstanceNum()    { return pcon.size(); }
    float getWidth()        { return width; }
    float getHeight()       { return height; }
    ofVec2f getCharPos()   { return charPos; }
    
    static void setFONT_SIZE(float f)       { FONT_SIZE = f; }
    static void setICON_SIZE(float f)       { ICON_SIZE = f; }
    static void setICON_DISTANCE(float f)   { ICON_DISTANCE = f; }
    static void setFONT_RANDOMNESS(float f) { FONT_RANDOMNESS = f;}
    static void setLETTER_SPACING(float f)  { LETTER_SPACING = f; font.setLetterSpacing(LETTER_SPACING);}
    static void setLINE_HEIGHT(float f)     { LINE_HEIGHT = f; font.setLineHeight(LINE_HEIGHT);}
    
    void setRandomWalk(bool b)   { bRandomWalk = b; }
    void setCharPos(ofVec2f v)  { charPos = v; }
    
    
public:
    
    struct Animation{
    public:
        enum AnimationType{ none=0, random=1, target=2, end=4 } type;
        bool randomWalk, globalPos, done;
        int exeFrame;
        float rw, rh;
        ofVec2f randPoint;
        Animation(AnimationType at, int ef, ofVec2f rp, float w, float h, bool rw, bool gp=false)
        :type(at), exeFrame(ef), randPoint(rp), rw(w), rh(h), randomWalk(rw), globalPos(gp), done(false){}
    };
    
    typedef pair<int, Animation*> AnimationData;
    typedef map<int, Animation*> AnimationCont;
    typedef AnimationCont::iterator AnimationContItr;
    AnimationCont animations;
    
    void updateAnimations();
    
    void setTargetAround(ofVec2f& p, float rw, float rh, bool randomWalk, bool globalPos);
    void setFinalTarget(bool randomWalk);
    
    void setRandomAnimation (int milliseconds, ofVec2f& p, float rw, float rh, bool _randomWalk, bool globalPos=false);
    void setFinalAnimation  (int milliseconds, bool randomWalk);
    void setEndAnimation    (int milliseconds, bool randomWalk);
    void clearAnimation(int waitTime);

    PCON& getPcon(){ return pcon; }
    pictoString * getParent(){ return parent; }

    void printAnimationList();
};
