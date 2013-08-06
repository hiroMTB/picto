//
//  picto.cpp
//  pictoTest_rev01
//
//  Created by matobahiroshi on 13/07/03.
//
//

#include "picto.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>
using namespace boost;

const ofColor picto::colors[colorTypeNum] = {
                                    ofColor(   0,  194,  229),     // water blue
                                    ofColor( 255,  205,   10),    // yellow
                                    ofColor( 165,   92,  192),    // purple
                                    ofColor(  10,  195,   92),     // light green
                                    ofColor( 250,   70,   83)      // light red
                                };

int picto::imgTypeNum = 0;
pImg * picto::imgs = NULL;

void picto::init(){
    loadImgFromAllstar(5, 10, 44);
}

void picto::loadImgFromAllstar(int row, int col, int num){
    ofImage all;
    int w = 128;
    int h = 128;
    
#ifdef USE_SVG
    ofLogError("loadFromAllstar", "can not use SVG for this function");
    ofExit();
#else
    bool loadok = all.loadImage("png/picto_all_star.png");
    if (!loadok) {
        cout << "Can not load " << "png/picto_all_star.png" << endl;
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
//            imgs[id].loadImage("png/picto_128.png");
            
            unsigned char * pix = new unsigned char[w*h*4];
  
            int index = 0;
            for(int py=0; py<h; py++){
                for(int px=0; px<w; px++){
//                    cout << "loading x " << x+px << ", y "<< y+py << endl;
                    color = all.getColor(x+px, y+py);
//                    int index = (px + py*w) * 4;
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

picto::picto(){
    ofPoint rand = ofPoint( ofRandom(50, 400), ofRandom(400,850), 0);
    target.set(rand);
}

picto::~picto(){
    
}


void picto::setRandom(){
    ofVec3f randp(ofRandom(ofGetWidth()*0.3, ofGetWidth()*0.7), ofRandom(ofGetHeight()*0.3, ofGetHeight()*0.7), 0);
    pos.set(randp + parent->getCharPos());
    scale = 0.3;
    
    colorType = (int)ofRandom(0, colorTypeNum);
    imgType = (int)ofRandom(0, imgTypeNum);
    
    vel.set(0, 0);
    acc.set(0, 0);
    
    mass = 1;
    spLength = ofRandom(10, 30);
    topSpeed = ofRandom(250, 500);
    minSpeed = topSpeed*0.07;
    K = ofRandom(30, 100);
    
    bArrive = false;
    arriveCount = ofRandom(1, 3);
    
    if(ofRandom(0, 100)>95){
        arriveCount = ofRandom(50, 70);
    }
    
//    phase = ofRandom(0, 360);
//    freq = ofRandom(0.5, 2);
    
    targetOffset.set(ofRandom(-1,1), ofRandom(-1,1));
    targetOffset.normalize();
    
    alpha = ofRandom(1, 20);
    
}

void picto::update(){
    
    target = target*0.99 + newTarget *0.01;
    
    if(alpha<255){
        alpha *= 1.02;
    }else if(alpha>255){
        alpha = 255;
    }else if(alpha <=0){
        alpha = 1;
    }
    
    bool rw = parent->getRandomWalk();
    
    if(rw){
//        gravitySim();
        springSim();
    }else{
        springSim();
    }
    
//    if(pos.x < 0 || ofGetWidth()<pos.x){
//        vel.x = -vel.x*0.9;
//    }
//
//    if(pos.y < 0 || ofGetHeight()<pos.y){
//        vel.y = -vel.y*0.9;
//    }
    
    
    speedControl();
    
//    vel *= 0.99;
    
//        sameAccSim();
//        targetSineMove();
}

void picto::springSim(){
    float dt = 0.0167;
    offsetRate = 0;
    
    ofVec3f dir = target+targetOffset*offsetRate - pos;
    
//    
//    if(bArrive && dir.length() < spLength){
//        ofVec3f force = dir/dir.length() * 1000000/dir.lengthSquared();
////        if(force.length()>18){
////            force.normalize();
////        }
//        vel += force;
//    }else if(bArrive && dir.length() > spLength){
//
//        arriveCount--;
//        vel*=0.7;
//    }else{
//
    
    ofVec3f accel = 0.01 * K * dir;
    
    if(!parent->getRandomWalk()){
        vel = vel*0.99 + (10 * dt* accel)*0.5;
        
//        if(vel.length()<10){
//            vel = vel.getNormalized() * 10;
//        }
    }else{
        vel += accel * dt;
    }
//    }
    
    pos += vel * dt;
    
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
    ofVec3f dir = target - pos;

    if(dir.length() >= spLength){
        acc = (1/sqrt(dir.length()) ) * dir.getNormalized();
        vel += acc;
    }
    
    
    pos += vel;
}

void picto::speedControl(){
    if(vel.length() > topSpeed){
        vel = vel.getNormalized() * topSpeed;
    }
}

void picto::draw(){
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    glScalef(scale, scale, scale);
    const ofColor &c = colors[colorType];
    ofSetColor(c.r, c.g, c.b, alpha);
    imgs[imgType].draw(0,0);
    glPopMatrix();
}

void picto::drawTarget(){
    
    ofVec3f home = target + targetOffset*offsetRate;
    
    glPushMatrix();{
        glTranslatef(home.x, home.y, home.z);
        ofFill();
        const ofColor &c = colors[colorType];
        ofSetColor(c.r, c.g, c.b, 255); //255-alpha);
        glPointSize(5);
        glBegin(GL_POINTS);
        glVertex3f(0,0,0);
        glEnd();

        glLineWidth(1);
        ofSetLineWidth(1);
        ofSetColor(c.r, c.g, c.b, 30); //60-alpha);
        glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(pos.x-home.x, pos.y-home.y, pos.z-home.z);
        glEnd();
    }glPopMatrix();
}

void picto::targetSineMove(){
//    offsetRate = sin(phase) * spLength;
//    phase += (TWO_PI * freq) /60.0;
}

void picto::setTarget(ofPoint p, int time){
//        Tweener.addTween(pos.x, p.x, time, &ofxTransitions::easeOutElastic);
//        Tweener.addTween(pos.y, p.y, time, &ofxTransitions::easeOutElastic);
    newTarget = p;
    bArrive = false;
    
    arriveCount = ofRandom(16, 18);
    
    if(ofRandom(0, 100)>98){
        arriveCount = 5;
    }
}













////////////////////////////////////////////////////////
//                                               //
//  pictoChar class                              //
//                                               //
///////////////////////////////////////////////////////

std::map<char, vector<ofPoint> > pictoChar::pointCharMap;

ofTrueTypeFont pictoChar::font;
int   pictoChar::fontSizeDefualt;
int   pictoChar::iconSizeDefault;
float pictoChar::overlapRateDefault;
float pictoChar::stringAlphaDefault;

pictoChar::pictoChar(char _c, ofVec3f _charPos)
:c(_c),
charPos(_charPos),
bRandomWalk(true),
stringAlpha(stringAlphaDefault)
{
    if(c!=' ' && c!='\n'){
        const map<char, vector<ofPoint> >::iterator itr = pointCharMap.find(c);
        if(itr != pointCharMap.end()){
            vector<ofPoint> &points = itr->second;
            int n = points.size();
            for(int i=0; i<n; i++){
                picto * p = new picto();
                pcon.push_back(p);
                p->setParent(this);
                p->setRandom();
            }
        }
    }
}

void pictoChar::initAlphabetFromFontdata(){
    string fontName = "HelveticaNeueCondensedBold.ttf";
    
    string alphabet;    //= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!\"#$%'()~|@`{}+-*=/<>?_.,[]";
    for(int i=32; i<128; i++){
        alphabet += (char)i;
    }

    fontSizeDefualt = 120 * 1.9;
    iconSizeDefault = 15 * 1.7;
    overlapRateDefault = 0.7;
    stringAlphaDefault = 100;

    const float originalSvgIconSize = 128;   // pixel

    font.loadFont("type/"+fontName, fontSizeDefualt, true, true, true);
    float spaceSize = font.getSpaceSize();
    float letterSpacing = font.getLetterSpacing();
    float lineHeight = font.getLineHeight();

    cout << "font set " << alphabet << endl;

    for(int i=0; i<alphabet.size(); i++){
        char c = alphabet.at(i);
        
        cout << c << " " << (int)c << "       ";

        float fw = font.stringWidth( ofToString(c) );
        float fh = font.stringHeight(ofToString(c));
        
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

void pictoChar::drawString(){
     float lineHeight = font.getLineHeight();
    ofSetColor(0, 0, 0, stringAlpha*=0.99);
    ofNoFill();
    font.drawStringAsShapes(ofToString(c), 0, lineHeight);
}


void pictoChar::update(){
    PITR itr = pcon.begin();
    for(; itr!=pcon.end(); itr++){
        (*itr)->update();
    }
}

void pictoChar::draw(){
    ofSetRectMode(OF_RECTMODE_CENTER);
    PITR itr = pcon.begin();
    for(; itr!=pcon.end(); itr++){
        (*itr)->draw();
    }
    ofSetRectMode(OF_RECTMODE_CORNER);
}

void pictoChar::drawTarget(){
    PITR itr = pcon.begin();
    for(; itr!=pcon.end(); itr++){
        (*itr)->drawTarget();
    }
}

void pictoChar::setTarget(vector<ofPoint> ps, bool _randomWalk, bool globalPos){
    int size = min(ps.size(), pcon.size());
    
    for(int i=0; i<size; i++){
        if(globalPos){
            pcon[i]->setTarget(ps[i], ofRandom(10, 30));
        }else{
            pcon[i]->setTarget(ps[i]+charPos, ofRandom(10, 30));
        }
    }
    
    bRandomWalk = _randomWalk;
    
    cout << "set target" << endl;
}

void pictoChar::setAnimation(char c, int milliseconds, bool _randomWalk){
    if(c!=' ' && c!='\n'){
        const map<char, vector<ofPoint> >::iterator itr = pointCharMap.find(c);
        if(itr != pointCharMap.end()){
            vector<ofPoint> &points = itr->second;
            setAnimation(points, milliseconds, false, false);
        }
    }
}


void pictoChar::setAnimation(vector<ofPoint> ps, int milliseconds, bool _randomWalk, bool globalPos){
    boost::thread t(boost::bind(&pictoChar::setAnimationCallback, this, ps, milliseconds, _randomWalk, globalPos));
}


void pictoChar::setAnimationCallback(vector<ofPoint> ps1, int milliseconds, bool _randomWalk, bool globalPos){
    boost::asio::io_service io;
    boost::asio::deadline_timer t1(io, boost::posix_time::milliseconds(milliseconds));
    t1.async_wait(boost::bind(&pictoChar::setTarget, this, ps1, _randomWalk, globalPos));
    io.run();
}


void pictoChar::clearAll(){
    pcon.clear();
    stringAlpha = stringAlphaDefault;
}

pictoChar::~pictoChar(){
    PITR itr = pcon.begin();
    for(; itr!=pcon.end(); itr++){
        delete (*itr);
    }
    pcon.clear();
}