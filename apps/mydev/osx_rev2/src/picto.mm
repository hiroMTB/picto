//
//  picto.cpp
//  pictoTest_rev01
//
//  Created by matobahiroshi on 13/07/03.
//
//

#include "testApp.h"
#include "picto.h"
#include "pictoChar.h"

#include <boost/bind.hpp>
#include <boost/thread.hpp>
using namespace boost;

int picto::totalPicto = 0;
int picto::iconSizeOriginal;
int picto::imgTypeNum = 0;
float picto::SPEED = -123;
float picto::ACCEL = -123;
int  picto::HOLD_TIME = -123;

const ofColor picto::colors[colorTypeNum] = {
                                    ofColor(   0,  194,  229),     // water blue
                                    ofColor( 255,  205,   10),    // yellow
                                    ofColor( 165,   92,  192),    // purple
                                    ofColor(  10,  195,   92),     // light green
                                    ofColor( 250,   70,   83)      // light red
                                };

pImg * picto::imgs = NULL;

void picto::init(){
    loadImgFromAllstar(5, 10, 44);
}

picto::picto()
:fixCount(0)
{
    float cw = testApp::getW() * 0.5;
    float ch = testApp::getH() * 0.5;
    float r = cw *0.3;
    ofVec2f rand = ofVec2f( ofRandom(cw-r, cw+r), ofRandom(ch, ch+r*2.0));
    target.set(rand);
    totalPicto++;
}


void picto::setRandom(){
    ofVec2f randp(ofRandom(testApp::getW(), testApp::getW()*1.2), ofRandom(testApp::getH()*0.4, testApp::getH()*0.7));
    pos.set(randp); //+ parent->getCharPos());
    
    colorType = (int)ofRandom(0, colorTypeNum);
    imgType = (int)ofRandom(0, imgTypeNum);
    
    vel.set(0, 0);
    acc.set(0, 0);
    
    spLength = ofRandom(12, 30);
    topSpeed = ofRandom(55, 85) * (SPEED+1);
    minSpeed = ofRandom(1, 10);
    
    K = ofRandom(8, 17) * (ACCEL+1);
    
    if(ofRandom(0, 100)>99){
        minSpeed = ofRandom(30, 50);
    }
    
    alpha = ofRandom(1, 10);
    
}

void picto::loadImgFromAllstar(int row, int col, int num){
    ofImage all;
    
    iconSizeOriginal = 64;
    int w = iconSizeOriginal;
    int h = iconSizeOriginal;
    string filePath = "png/picto_all_star_" + ofToString(iconSizeOriginal,0)+".png";
    
#ifdef USE_SVG
    ofLogError("loadFromAllstar", "can not use SVG for this function");
    ofExit();
#else
    bool loadok = all.loadImage(filePath);
    if (!loadok) {
        cout << "Can not load " <<  filePath << endl;
    }
#endif
    
    row = 5;
    col = 10;
    imgTypeNum = num;
    imgs = new pImg[imgTypeNum];
    
    ofColor color;
    
    for(int i=0; i<=row; i++){
        for(int j=0; j<=col; j++){
            int id = i*col + j;
            if (id>=imgTypeNum) {
                break;
            }
            
            int x = j*w;
            int y = i*h;

            imgs[id].allocate(w, h, OF_IMAGE_COLOR_ALPHA);
            
            unsigned char * pix = new unsigned char[w*h*4];
  
            int index = 0;
            for(int py=0; py<h; py++){
                for(int px=0; px<w; px++){
                    color = all.getColor(x+px, y+py);
                    pix[index+0] = color.r;
                    pix[index+1] = color.g;
                    pix[index+2] = color.b;
                    pix[index+3] = color.a;

                    index+=4;
                }
            }

//            imgs[id].getPixelsRef().setFromPixels(pix, w, h, OF_IMAGE_COLOR_ALPHA);
            imgs[id].setFromPixels(pix, w, h, OF_IMAGE_COLOR_ALPHA);
        }
    }
    
    all.clear();
}

void picto::loadImgFromSeparateFile(){

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



picto::~picto(){
    totalPicto--;
}



void picto::update(){
    
    if(alpha<255){
        alpha *= 1.02;
    }else if(alpha>255){
        alpha = 255;
    }else if(alpha <=0){
        alpha = 1;
    }
    if(parent->getClearance()){
        alpha *= 0.98;
    }

    // reset
    acc.set(0,0);
    sep.set(0,0);
    
    dir = target - pos;
    target = target*0.99 + newTarget *0.01;
    
    float dt = 0.0167;

    if(parent->getRandomWalk()){
        // spring
        acc = springSim()*1.2;
        
        bool bFlock = false;
        // flock
        if(bFlock){
            vector<picto*> friends1 = parent->getParent()->seek(pos, 25);
            sep = separate(friends1, 20);
            acc += sep*2.0;
            
            vector<picto*> friends2 = parent->getParent()->seek(pos, 50);
            ali = align(friends2, 50);
            acc += ali;
        
            coh = cohesion(friends2, 50);
            acc += coh;
        }
        vel += acc;
    }else{
        if(vel.length()>minSpeed){
            acc = springSim()*4.0;
            
            vel *= 0.98;
            vel += acc;
        }else{
            if(fixCount<50){
                fixCount++;
                float sp = vel.length();
                vel = dir.getNormalized() * sp;
              
            }
            acc = springSim();
            vel+=acc;
        }
    }
    
    vel.limit(topSpeed);
    
    // POS
    pos += vel * dt;
    
//    borders();
    
//    speedControl();
}

ofVec2f picto::springSim(){
    return K*0.000167*dir;
}

ofVec2f picto::gravitySim(){
    ofVec2f force(0,0);
    if(dir.length() >= spLength){
        force = (1/sqrt(dir.length()) ) * dir.getNormalized();
    }
    return force;
}

float maxForce = 5.0;

ofVec2f picto::separate(vector<picto*> friends, float distance){

    int n = friends.size();
    if(n<=0) return;
    ofVec2f ret(0,0);
    
    for(int i=0; i<n; i++){
        picto * pic2 = friends[i];
        ofVec2f& pos2 = pic2->getPos();

        ofVec2f diff = pos - pos2;
        diff.normalize();
        diff /= distance;
        ret += diff;
    }

    ret /= (float)n;

//    if(ret.lengthSquared()>0){
        ret.normalize();
        ret *= topSpeed;
        ret -= vel;
        ret.limit(maxForce);
//    }

    return ret;
}

ofVec2f picto::align(vector<picto*> friends, float distance){
    int n = friends.size();
    if(n<=0) return ofVec2f(0,0);
    
    ofVec2f sum(0,0);
    ofVec2f force(0,0);
    
    for(int i=0; i<n; i++){
        picto * pic2 = friends[i];
        sum += pic2->getVel();
    }
    
    sum /= (float)n;
    sum.normalize();
    sum *= topSpeed;
    force = sum - vel;
    force.limit(maxForce);
    return force;
}

ofVec2f picto::cohesion(vector<picto*> friends, float distance){
    int n = friends.size();
    if(n<=0) return ofVec2f(0,0);
    
    ofVec2f sum(0,0);
    ofVec2f force(0,0);
    
    for(int i=0; i<n; i++){
        picto * pic2 = friends[i];
        sum += pic2->getPos();
    }
    
    sum /= (float) n;
    ofVec2f desi = sum - pos;
    desi.normalize();
    desi *= topSpeed;
    force = desi -vel;
    force.limit(maxForce);
    
    return force;
}


void picto::borders(){
    int width = testApp::getW();
    int height = testApp::getH();
    int r = iconSizeOriginal/2;
    if (pos.x < -r) pos.x = width+r;
    if (pos.y < -r) pos.y = height+r;
    if (pos.x > width+r) pos.x = -r;
    if (pos.y > height+r) pos.y = -r;
}
void picto::speedControl(){
    if(vel.length() > topSpeed){
        vel = vel.getNormalized() * topSpeed;
    }
}

void picto::draw(){
    glPushMatrix();
    glTranslatef(pos.x, pos.y, 0);
    glScalef(scale, scale, scale);
    const ofColor &c = colors[colorType];
    ofSetColor(c.r, c.g, c.b, alpha);
    imgs[imgType].draw(0,0);
    
    // draw acc
//    glLineWidth(1);
//    ofSetLineWidth(1);
//    glBegin(GL_LINES);
//    ofSetColor(255, 0, 0, 255);
//    glVertex3f(0,0,0);
//    glVertex3f(acc.x*15.0, acc.y*15.0, 0);
//
//    glEnd();

    glPopMatrix();
}

void picto::drawTarget(){
    
    ofVec2f &home = target;
    
    glPushMatrix();{
        ofFill();
        const ofColor &c = colors[colorType];
        ofSetColor(c.r, c.g, c.b);
        glPointSize(4);
        glBegin(GL_POINTS);
        glVertex3f(home.x, home.y,0);
        glEnd();

//        glLineWidth(1);
//        ofSetLineWidth(1);
//        ofSetColor(c.r, c.g, c.b, 10); //60-alpha);
//        glBegin(GL_LINES);
//        glVertex3f(0,0,0);
//        glVertex3f(pos.x-home.x, pos.y-home.y, 0);
//        glEnd();
    }glPopMatrix();
}

void picto::setNewTarget(ofVec2f p, int time){
    newTarget = p;
}










