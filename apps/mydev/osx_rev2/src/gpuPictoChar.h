//
//  gpuPictoChar.h
//  osx_picto
//
//  Created by matobahiroshi on 2013/08/30.
//
//

#pragma once

#include "ofMain.h"

class gpuPictoString;
class gpuPicto;

class gpuPictoChar{
  

public:

    bool    bSpread;
    int     spreadFrame;
    int     firstIndex;
    int     numPicto;
    char    c;
    
    ofVec2f charPos;
    ofFbo   fbo;
    
    gpuPictoString * parent;
    
    typedef vector<gpuPicto*> GPCON;
    typedef GPCON::iterator GPITR;
    GPCON gpcon;

    gpuPictoChar(char c, float x, float y, gpuPictoString * _parent);
    bool update();
    void getFinalTarget(ofTrueTypeFont& font, float fontScale, float res, float rand, vector<float>& data);
};