//
//  pictoString.h
//  osx_picto_rev02
//
//  Created by matobahiroshi on 2013/08/27.
//
//

#pragma once

#include "ofMain.h"
#include "pictoChar.h"

class pictoString{

public:
    typedef vector<pictoChar*> PICTO_STR;
    typedef PICTO_STR::iterator PICTO_STR_ITR;
    
private:
    PICTO_STR pchars;
    ofVec2f offsetPos;
    string text;
    
public:
    pictoString();
    ~pictoString();
    
    void update();
    void draw();
    void drawPreview();
    
    void makeAnimation();
    void clearAll();
    void clearFromPictoString(pictoChar * c);
    vector<picto*> seek(const ofVec2f& p, float distance);

public:
    void setText(string s){ text = s; }
    
};