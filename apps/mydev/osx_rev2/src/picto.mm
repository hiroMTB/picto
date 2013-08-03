//
//  picto.cpp
//  pictoTest_rev01
//
//  Created by matobahiroshi on 13/07/03.
//
//

#include "picto.h"

const ofColor picto::colors[colorPallettNum] = {
                                    ofColor(   0,  194,  229),     // water blue
                                    ofColor( 255,  205,   10),    // yellow
                                    ofColor( 165,   92,  192),    // purple
                                    ofColor(  10,  195,   92),     // light green
                                    ofColor( 250,   70,   83)      // light red
                                };

int picto::imgTypeNum = 0;
pImg * picto::imgs = NULL;

void picto::init(){

    ofDirectory dir;
    
#ifdef USE_SVG
    string imgFormat = "svg";
#else
    string imgFormat = "png";
#endif
    dir.allowExt(imgFormat);
    imgTypeNum = dir.listDir(imgFormat);
    cout << "found " <<  ofToString(imgTypeNum) << " " +imgFormat + " files" << endl;
    
    imgs = new pImg[imgTypeNum];
    
    for(int i=0; i<imgTypeNum; i++){
        string path = dir.getPath(i);
#ifdef USE_SVG
        imgs[i].load(path);
#else
        imgs[i].loadImage(path);
#endif
        cout << "load "+imgFormat + " files" << path << ", as type " << ofToString(i) << endl;
    }
}

picto::picto(){
    setRandom();
}

picto::~picto(){}


void picto::setRandom(){
    pos.set(ofRandom(ofGetWidth()*0.7, ofGetWidth()*1.2), ofRandom(-ofGetHeight()*0.1, ofGetHeight()*1.1), 0);
    scale = 0.3;
    
    colorType = (int)ofRandom(0, colorPallettNum);
    imgType = (int)ofRandom(0, imgTypeNum-1);
    
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

void picto::update(){
        springSim();
//        sameAccSim();
//        targetSineMove();
}

void picto::springSim(){
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

void picto::sameAccSim(){
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



void picto::gravitySim(){
//
//        ofVec3f dir = target - pos;
//        float mass = 1;
//        float force = 100000 / dir.lengthSquared();
//        float g = force;
//        speedf = speedf + g;
//
//        pos = pos + speedf*dir/dir.length();    
}

void picto::draw(){
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    glScalef(scale, scale, scale);
    const ofColor &c = colors[colorType];
    ofSetColor(c.r, c.g, c.b, 255);
    imgs[imgType].draw(0,0);
    glPopMatrix();
}

void picto::drawTarget(){
    
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

void picto::targetSineMove(){
    offsetRate = sin(phase) * spLength;
    phase+=PI*0.005;
}

void picto::setTarget(ofPoint p, int time){
//        Tweener.addTween(pos.x, p.x, time, &ofxTransitions::easeOutElastic);
//        Tweener.addTween(pos.y, p.y, time, &ofxTransitions::easeOutElastic);
    target = p;
    bArrive = false;
    
    arriveCount = ofRandom(16, 18);
    
    if(ofRandom(0, 100)>98){
        arriveCount = 5;
    }
}



//
//  pictoChar class
//
std::map<char, vector<ofPoint> > pictoChar::pointCharMap;

ofTrueTypeFont pictoChar::font;
int   pictoChar::fontSizeDefualt;
int   pictoChar::iconSizeDefault;
float pictoChar::overlapRateDefault;


