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

//#define USE_SVG

#ifdef USE_SVG
typedef ofxSVG pImg;
#else
typedef ofImage pImg;
#endif

class picto{
  
private:
    static int imgTypeNum;
    static pImg * imgs;
    
    
    ofVec3f pos, target, targetOffset;
    ofVec3f acc, vel, dir;

    ofQuaternion quat;

    float angle;
    float offsetRate;
    float scale;
    float topSpeed, minSpeed;
    float mass;
    float spLength;
    float K;
    
    bool bArrive;
    int colorType;
    int imgType;
    
    float arriveCount;
    float phase;
    
public:
    static const int colorPallettNum = 5;
    static const ofColor colors[colorPallettNum];
    
    static void init();
    
    picto();
    ~picto();
    
    void setRandom();

    void update();
    
    void targetSineMove();
    
    void springSim();
    void sameAccSim();
    void gravitySim();
    
    void draw();
    void drawTarget();
    
    void setPos(ofPoint p){ pos = p; }
    void setScale(float s){ scale = s; }
    void setAngle(float a){ angle = a; }
    void setColorType(int type){ colorType = type%colorPallettNum; }
    void setImgType(int i){ imgType = i; }
    
    void setTarget(ofPoint p, int time=1000);
    
    
};

class pictoChar{
    
private:
    static std::map<char, vector<ofPoint> > pointCharMap;
    
    char c;
    static ofTrueTypeFont font;
    static int   fontSizeDefualt;
    int         fontSize;

    static int   iconSizeDefault;
    int         iconSize;

    static float overlapRateDefault;
    float       overlapRate;
    
public:

    static void initAlphabetFromFontdata(){
        string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWZYGabcdefghijklmnopqrstuvwxyz1234567890";
        
        fontSizeDefualt = 120 *2.5;
        iconSizeDefault = 15 * 2;
        overlapRateDefault = 0.8;
        const float originalSvgIconSize = 100;   // pixel
        
        font.loadFont("type/HelveticaNeueCondensedBold.ttf", fontSizeDefualt, true, false, true);
        float spaceSize = font.getSpaceSize();
        float letterSpacing = font.getLetterSpacing();
        float lineHeight = font.getLineHeight();

        for(int i=0; i<alphabet.size(); i++){
            char c = alphabet.at(i);
            float fw = font.stringWidth( ofToString(c) );
            float fh = font.stringHeight(ofToString(c));
            cout << "make alphabet data of " << c << endl;
            
            ofFbo * fbo = new ofFbo();
            {
                // make fbo
                fbo->allocate(fw, fh);
                fbo->begin();
                ofSetColor(0, 0, 0);
                ofRect(0, 0, fbo->getWidth(), fbo->getHeight());

                ofSetColor(255, 0, 0);
                ofRectangle bounds = font.getStringBoundingBox( ofToString(c), 0, 0);
                font.drawStringAsShapes(ofToString(c), -bounds.x, -bounds.y);
                fbo->end();
            }
            
            ofPixels pix;
            ofTexture targetTex;
            pix.allocate(fbo->getTextureReference().getWidth(),fbo->getTextureReference().getHeight(), OF_PIXELS_RGBA);
            targetTex.allocate(pix);
            fbo->readToPixels(pix);
            targetTex.loadData(pix);
            
            int pw = pix.getWidth();
            int ph = pix.getHeight();
            float res = iconSizeDefault * overlapRateDefault;
            
            vector<ofPoint> points;
            
            for(int sx=res/2; sx<pw; sx+=res){
                for(int sy=res/2; sy<ph; sy+=res){
                    ofColor col = pix.getColor(sx, sy);
                    if(col.r > 200) {
                       float rand = iconSizeDefault/10;
                       points.push_back(ofPoint(sx, sy,0));
                    }
                }
            }
            
            pointCharMap.insert( pair<char, vector<ofPoint> >(c, points) );
            
            delete fbo;
        }
    }
    
    pictoChar(char _c):c(_c){
        if(c!=' ' && c!='\n'){
            const map<char, vector<ofPoint> >::iterator itr = pointCharMap.find(c);
            if(itr != pointCharMap.end()){
                vector<ofPoint> &point = itr->second;
                int n = point.size();
                for(int i=0; i<n; i++){
                    picto * p = new picto();
                    p->setTarget(point[i]);
                    pcon.push_back(p);
                }
            }
        }
    };
    
    
    
    typedef vector<picto*> PCON;
    typedef PCON::iterator PITR;
    PCON pcon;
    
    void clearAll(){
        pcon.clear();
    }
    
    void update(){
        PITR itr = pcon.begin();
        for(; itr!=pcon.end(); itr++){
            (*itr)->update();
        }
    }
    
    void draw(){
        ofSetRectMode(OF_RECTMODE_CENTER);
        PITR itr = pcon.begin();
        for(; itr!=pcon.end(); itr++){
            (*itr)->draw();
        }
        ofSetRectMode(OF_RECTMODE_CORNER);
    }

    void drawTarget(){
        PITR itr = pcon.begin();
        for(; itr!=pcon.end(); itr++){
            (*itr)->drawTarget();
        }
    }

    void setTarget(vector<ofPoint> ps){
        int size = min(ps.size(), pcon.size());
        
        for(int i=0; i<size; i++){
            pcon[i]->setTarget(ps[i], ofRandom(10, 30));
        }
    }
    
    int getInstanceNum(){
        return pcon.size();
    }

};


