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

class picto{
  
private:
    ofxSVG * svg;
    ofImage * img;
    
    
    ofPoint pos;
    float scale;
    float angle;
    ofQuaternion quat;
    
    ofVec3f vel;
    ofColor color;
    ofPoint p;
    
public:
    
    picto(){
        setRandom();
    }
    
    
    ~picto(){}
    
    // should be called from pictoManager class
    void draw(){
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glScalef(scale, scale, scale);
        glRotatef(angle, 0, 0, 1);
        svg->draw();
        //img->draw(0,0);
        
        glPopMatrix();
        
    }
    
    void setRandom(){
        setPos(ofPoint(ofRandom(-ofGetWidth()/2, ofGetWidth()/2), ofRandom(-ofGetHeight()/2, ofGetHeight()/2), 0));
        setVel(ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), 0));
    }

    void setPos(ofPoint p){ pos = p; }
    void setVel(ofVec3f v){ vel = v; }
    void setScale(float s){ scale = s; }
    void setAngle(float a){ angle = a; }
    void setSVG(ofxSVG* s){ svg = s;  }
    void setImage(ofImage * i){ img = i; }
    
    void setTarget(ofPoint p, int time=1000){
        Tweener.addTween(pos.x, p.x, time, &ofxTransitions::easeOutBounce);
        Tweener.addTween(pos.y, p.y, time, &ofxTransitions::easeOutBounce);
    }
    
    
};

class pictoManager{
    
private:
    int svgMax;
    
    
    ofxSVG *svgLib;
    ofImage * imgLib;
    
public:
    pictoManager(){};
    ~pictoManager(){};
    
    
    void init(){
        {
            ofDirectory dir;
            
            dir.allowExt("svg");                // only count ***.svg
            svgMax = dir.listDir("svg");
            cout << "found " <<  ofToString(svgMax) << " svg files" << endl;
            
            svgLib = new ofxSVG[svgMax];
            
            for(int i=0; i<svgMax; i++){
                string path = dir.getPath(i);
                svgLib[i].load(path);
                cout << "load svg file " << path << ", as type " << ofToString(i) << endl;
            }
        }
        {
            ofDirectory dir;
            
            dir.allowExt("png");                // only count ***.svg
            svgMax = dir.listDir("img");
            cout << "found " <<  ofToString(svgMax) << " svg files" << endl;
            
            imgLib = new ofImage[svgMax];
            
            for(int i=0; i<svgMax; i++){
                string path = dir.getPath(i);
                imgLib[i].loadImage(path);
                cout << "load img file " << path << ", as type " << ofToString(i) << endl;
            }
        }
    }
    
    typedef vector<picto*> PCON;
    typedef PCON::iterator PITR;
    PCON pcon;
    
    void add(picto * p, int svgType){
        p->setSVG(&svgLib[svgType]);
        p->setImage(&imgLib[svgType]);
        pcon.push_back(p);
        
    }
    
    void clearAll(){
        pcon.clear();
    }
    
    void draw(){
        PITR itr = pcon.begin();
        for(; itr!=pcon.end(); itr++){
            (*itr)->draw();
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


