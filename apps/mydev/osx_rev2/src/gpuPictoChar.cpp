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

gpuPictoChar::~gpuPictoChar(){
    GPITR itr = gpcon.begin();
    for(; itr!=gpcon.end(); itr++){
        gpuPicto * gp = (*itr);
        delete gp;
        gp = 0;
    }
    gpcon.clear();
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
    
    charProps cp = font.getCharProps(c);
    
    float lineHeight = font.getLineHeight() * fontScale;
    float setWidth   = cp.setWidth * fontScale;
    //float height     = cp.height;
    //float topExtent  = cp.topExtent;
    //float width      = cp.width;
    //float leftExtent = cp.leftExtent;
    
    int pixW = setWidth;
    int pixH = lineHeight * 1.5;
    
//    printf("\n%c\nwidth= %0.1f\nsetWidth= %0.1f\nheight= %0.1f\ntopExtent= %0.1f\nleftExtent%0.1f\n",
//    c, w, fw, fh, te, le);
    
//    ofFbo * fbo = new ofFbo();
    {
        fbo.allocate(pixW, pixH);
        fbo.begin();
        ofFill();
        ofSetColor(0, 0, 0);
        ofRect(0, 0, pixW, pixH);
        ofSetColor(250, 0, 0);
        ofScale(fontScale, fontScale);
        font.drawStringAsShapes(ofToString(c), 0, lineHeight*1.2/fontScale);
        fbo.end();
    }
    
    ofPixels pix;
    ofTexture tex;
    pix.allocate(pixW, pixH, OF_PIXELS_RGBA);
    tex.allocate(pix);
    fbo.readToPixels(pix);
    tex.loadData(pix);
    
    int count = 0;
    for(int sy=res/2; sy<pixH; sy+=res){
        for(int sx=res/2; sx<pixW; sx+=res){
            ofColor col = pix.getColor(sx, sy);
            if(col.r > 50) {
                data.push_back(sx+ ofRandom(-rand, rand));
                data.push_back(sy+ ofRandom(-rand, rand));
                data.push_back(0);
                count++;
//                gpuPicto::totalPicto++;
            }
        }
    }
    
//    delete fbo;

    numPicto = count;
}


