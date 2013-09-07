//
//  attractor.h
//  osx_picto
//
//  Created by matobahiroshi on 2013/08/29.
//
//

#pragma once

#include "ofMain.h"
#include "testApp.h"
#include "ofxTweener.h"

class attractor{
    
public:
    typedef pair<int, ofVec2f> ATR_PAIR;
    typedef map<int, ofVec2f> ATR_CON;
    typedef ATR_CON::iterator ATR_ITR;
    
private:
    attractor();
    static attractor * instance;
    static ATR_CON atrs;
    
    static ofVec2f pos;
    static bool     bOn;
    
    static float startTime;
    static float duration;
    static ofVec2f startPos;
    static ofVec2f newPos;
    
public:

    static attractor * getInstance(){ return instance; }
    static void init(){ if(!instance) instance = new attractor(); }

    static void update();
    static void draw();
    
    static void addAttraction(int millisec, const ofVec2f& p);
    
    static bool getOn(){ return bOn; }
    static const ofVec2f& getPos(){ return pos; }
    
    static void setOn(bool b){bOn = b; }
  
    static ofxTweener tweener;
    
    
};