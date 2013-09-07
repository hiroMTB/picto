//
//  gpuPictoChar.cpp
//  osx_picto
//
//  Created by matobahiroshi on 2013/08/30.
//
//

#include "gpuPictoChar.h"
#include "gpuPictoString.h"
#include "gpuPicto.h"

gpuPictoChar::gpuPictoChar(char _c, float x, float y, gpuPictoString * _parent)
:c(_c), parent(_parent){
    charPos.x = x;
    charPos.y = y;
    spreadFrame = 500;
    bSpread = false;
}



bool gpuPictoChar::update(){
    
    if(!bSpread){
        float now = ofGetFrameNum();

        if (spreadFrame<=now){
            bSpread = true;
            //cout << "update char" << endl;
            return true;
        }
    }
    return false;
}


void gpuPictoChar::getFinalTarget(ofTrueTypeFont& font, float fontScale, float res, float rand, vector<float>& data){

    firstIndex = gpuPicto::totalPicto;
    
    float fw = font.getCharProps(c).setWidth * fontScale;
    float fh = font.getLineHeight() * fontScale;
    
//    ofFbo * fbo = new ofFbo();
    {
        fbo.allocate(fw, fh*1.2);
        fbo.begin();
        ofFill();
        ofSetColor(0, 0, 0);
        ofRect(0, 0, fw, fh*1.2);
        ofSetColor(255, 0, 0);
        ofScale(fontScale, fontScale);
        font.drawStringAsShapes(ofToString(c), 0, fh/fontScale);
        fbo.end();
    }
    
    ofPixels pix;
    ofTexture tex;
    pix.allocate(fw, fh*1.2, OF_PIXELS_RGBA);
    tex.allocate(pix);
    fbo.readToPixels(pix);
    tex.loadData(pix);
    
    int pw = fw;
    int ph = fh*1.2;

    int count = 0;
    for(int sx=res/2; sx<pw; sx+=res){
        for(int sy=res/2; sy<ph; sy+=res){
            ofColor col = pix.getColor(sx, sy);
            if(col.r > 200) {
                data.push_back(sx+ ofRandom(-rand, rand));
                data.push_back(sy+ ofRandom(-rand, rand));
                data.push_back(0);
                count++;
                gpuPicto::totalPicto++;
            }
        }
    }
    
//    delete fbo;

    numPicto = count;
}


