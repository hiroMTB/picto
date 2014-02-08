//
//  attractor.cpp
//  osx_picto
//
//  Created by matobahiroshi on 2013/08/29.
//
//

#include "attractor.h"
#include "testApp.h"

attractor * attractor::instance = NULL;

bool attractor::bOn = false;
float attractor::startTime = 0;
float attractor::duration = 0;
ofVec2f attractor::pos(0,0);
ofVec2f attractor::newPos(0,0);
ofVec2f attractor::startPos(0,0);

attractor::ATR_CON attractor::atrs;

attractor::attractor(){
    bOn = true;
    
    float w = testApp::getW();
    float h = testApp::getH();
    pos.set(0.5, 0.5);
    newPos.set(0.5, 0.5);
    startPos.set(0.5,0.5);
}

float easeInOutCubic(float t,float b , float c, float d){
	if ((t/=d/2) < 1) return c/2*t*t*t + b;
	return c/2*((t-=2)*t*t + 2) + b;
}

void attractor::update(){
    
    if(bOn){
        int now = ofGetFrameNum();
        
        ATR_ITR itr = atrs.begin();
        for (; itr!=atrs.end(); ) {
            int frame = itr->first;
            if(now > frame){
                newPos = itr->second;
                atrs.erase(itr++);
                
                float dist = newPos.distance(pos);
                float sec = dist * 30.0;
            }else{
                ++itr;
            }
        }
        
        //if(pos.distance(newPos)<0.08){
        //    pos += (newPos - pos).normalize()*0.002;
        //}else{
        //    pos = pos*0.977 + newPos*0.023;
        //}
        
        pos = pos*0.977 + newPos*0.023;
    }
}

void attractor::draw(){
//    if(testApp::getDebugDraw()){
        if(bOn){
            ofFill();
            glColor4f(0.1, 0.3, 1, 1);
            glPointSize(12);
            glBegin(GL_POINTS);
            glVertex3f(pos.x, pos.y, 0);
            glEnd();
        }
//  }
}

void attractor::addAttraction(int millisec, const ofVec2f& p){
    int frame = ofGetFrameNum() + (float)millisec/1000.0*60.0;
    atrs.insert(ATR_PAIR(frame, p));
}
