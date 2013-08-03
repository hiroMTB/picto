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
    pImg * img;
    
    ofVec3f pos, target;
    ofVec3f targetOffset;
    float offsetRate;
    
    ofVec3f acc, vel, dir;
    
    float scale;
    ofQuaternion quat;
    float angle;

    int colorType;
    float topSpeed, minSpeed;
    float mass;
    float spLength;
    float K;
    
    bool bArrive;
    float arriveCount;
    float phase;
    
public:
    static const int colorPallettNum = 5;
    static const ofColor colors[colorPallettNum];
    
    picto(){
        setRandom();
    }
    
    void setRandom(){
        pos.set(ofRandom(-ofGetWidth()/2, ofGetWidth()/2), ofRandom(-ofGetHeight()/2, ofGetHeight()/2), 0);
        
        setColorType((int)ofRandom(0, colorPallettNum));
        vel.set(0, 0);
        acc.set(0, 0);
        
        mass = 1;
        spLength = ofRandom(10, 30);
        topSpeed = ofRandom(250, 500);
        minSpeed = topSpeed*0.08;
        K = 100;
        bArrive = false;
        arriveCount = ofRandom(1, 3);

        if(ofRandom(0, 100)>98){
            arriveCount = 50;
        }
        
        phase = ofRandom(0, 360);
        targetOffset.set(ofRandom(-1,1), ofRandom(-1,1));
        targetOffset.normalize();
    }
    
    ~picto(){}
    
    void update(){
        springSim();
//        sameAccSim();
//        targetSineMove();
    }
    
    void targetSineMove(){
        offsetRate = sin(phase) * spLength;
        phase+=PI*0.005;
    }
    
    void springSim(){
        float dt = 0.0167;
        offsetRate = 0;
        
        ofVec3f dir = target+targetOffset*offsetRate - pos;

        if(bArrive && arriveCount <= 0){
            ofVec3f force = dir/dir.length() * 1000000/dir.lengthSquared();
            if(force.length()>18){
                force.normalize();
            }
            vel += force;
        }else if(bArrive && arriveCount > 0){
            arriveCount--;
            vel*=0.7;
        }else{
        
            ofVec3f accel = K * dir;
            vel += accel * dt;
            if(vel.length() >= topSpeed){
                vel = dir.getNormalized() * topSpeed;
            }
            
            if(spLength > dir.length()){
                bArrive = true;
                vel *= 0.9;
//                arriveCount = ;
            }
        }
        

    
        pos += vel * dt;
        
        
//        cout << "acc: "<< accel.length();
//        cout << "  vel: " << vel.length() << endl;
    }
    
    void sameAccSim(){
        dir = target+targetOffset*offsetRate - pos;
        acc = dir.getNormalized() * 0.2;
        vel += acc;
        
        if(vel.length() < topSpeed){
        }else{
            vel = vel.normalize() * topSpeed;
        }
        
        if(dir.length() < spLength){
//            vel.normalize();
//            vel.set(0, 0);
            pos = target + targetOffset*offsetRate;
        }else{
        
            pos += vel;
        }

    }
    
    void gravitySim(){
//    
//        ofVec3f dir = target - pos;
//        float mass = 1;
//        float force = 100000 / dir.lengthSquared();
//        float g = force;
//        speedf = speedf + g;
//        
//        pos = pos + speedf*dir/dir.length();
        
    }
    
    // should be called from pictoManager class
    void draw(){
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glScalef(scale, scale, scale);
        const ofColor &c = colors[colorType];
        ofSetColor(c.r, c.g, c.b, 255);
        img->draw(0,0);
        glPopMatrix();
    }
    
    void drawTarget(){
        
        ofVec3f home = target + targetOffset*offsetRate;
        
        glPushMatrix();
        glTranslatef(home.x, home.y, home.z);
        ofFill();
        const ofColor &c = colors[colorType];
        ofSetColor(c.r, c.g, c.b, 155);
        glPointSize(5);
        glBegin(GL_POINTS);
        glVertex3f(0,0,0);
        glEnd();
        glPopMatrix();
        
        glPushMatrix();
        ofSetLineWidth(1);
        ofLine(home.x, home.y, pos.x , pos.y);
        glPopMatrix();
    }
    


    void setPos(ofPoint p){ pos = p; }
    void setScale(float s){ scale = s; }
    void setAngle(float a){ angle = a; }
    void setColorType(int type){ colorType = type%colorPallettNum; }
    void setImg(pImg* i){ img = i; }
    
    void setTarget(ofPoint p, int time=1000){
//        Tweener.addTween(pos.x, p.x, time, &ofxTransitions::easeOutElastic);
//        Tweener.addTween(pos.y, p.y, time, &ofxTransitions::easeOutElastic);
        target = p;
        bArrive = false;
        
        arriveCount = ofRandom(16, 18);
        
        if(ofRandom(0, 100)>98){
            arriveCount = 5;
        }
    }
    
    
};

class pictoManager{
    
private:
    int svgMax;
    
    pImg      * imgLib;
    
public:
    pictoManager(){};
    ~pictoManager(){};
    
    
    void init(){
        ofDirectory dir;
        
#ifdef USE_SVG
        string imgFormat = "svg";
#else
        string imgFormat = "png";
#endif
        dir.allowExt(imgFormat);
        svgMax = dir.listDir(imgFormat);
        cout << "found " <<  ofToString(svgMax) << " " +imgFormat + " files" << endl;
        
        imgLib = new pImg[svgMax];
        
        for(int i=0; i<svgMax; i++){
            string path = dir.getPath(i);
#ifdef USE_SVG
            imgLib[i].load(path);
#else
            imgLib[i].loadImage(path);
#endif
            cout << "load "+imgFormat + " files" << path << ", as type " << ofToString(i) << endl;
        }
    }
    
    typedef vector<picto*> PCON;
    typedef PCON::iterator PITR;
    PCON pcon;
    
    void add(picto * p, int imgType){
        p->setImg(&imgLib[imgType]);
        pcon.push_back(p);
        
    }
    
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
    
    void drawOriginalPicto(int type){
        imgLib[type].draw(0,0);
    }
};


