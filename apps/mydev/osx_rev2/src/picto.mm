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
    setRandom();
}

picto::~picto(){
    
}


void picto::setRandom(){
    pos.set(ofRandom(ofGetWidth()*0.7, ofGetWidth()*1.2), ofRandom(-ofGetHeight()*0.1, ofGetHeight()*1.1), 0);
    scale = 0.3;
    
    colorType = (int)ofRandom(0, colorTypeNum);
    imgType = (int)ofRandom(0, imgTypeNum);
    
    vel.set(0, 0);
    acc.set(0, 0);
    
    mass = 1;
    spLength = ofRandom(10, 30);
    topSpeed = ofRandom(250, 500);
    minSpeed = topSpeed*0.07;
    K = ofRandom(5, 100);
    
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
    if(alpha<255){
        alpha *= 1.02;
    }else if(alpha>255){
        alpha = 255;
    }else if(alpha <=0){
        alpha = 1;
    }
    
    bool rw = parent->getRandomWalk();
    
    if(rw){
        gravitySim();
    }else{
        //springSim();
        gravitySim();
        
    }
    
//    if(pos.x < 0 || ofGetWidth()<pos.x){
//        vel.x = -vel.x*0.9;
//    }
//
//    if(pos.y < 0 || ofGetHeight()<pos.y){
//        vel.y = -vel.y*0.9;
//    }
    
    
//    speedControl();
    
    vel *= 0.99;
    
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
    ofVec3f dir = target - pos;
    
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
        if(dir.length() >= spLength){
            acc = (50 / dir.length()) * dir.getNormalized();
            vel += acc;
        }else{
            bArrive = true;
            vel *= 0.9;
        }
    }
    
//    speedControl();
    
    pos += vel;
    
}

void picto::speedControl(){
    if(vel.length() > topSpeed){
        vel = vel.getNormalized() * topSpeed;
    }else if(vel.length() < minSpeed){
        vel = vel.getNormalized() * minSpeed;
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
        ofSetColor(c.r, c.g, c.b, 255-alpha);
        glPointSize(7);
        glBegin(GL_POINTS);
        glVertex3f(0,0,0);
        glEnd();

        glLineWidth(1);
        ofSetLineWidth(1);
        ofSetColor(c.r, c.g, c.b, 60-alpha);
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
float pictoChar::stringAlphaDefault;

pictoChar::pictoChar(char _c)
:c(_c),
bRandomWalk(true),
stringAlpha(stringAlphaDefault)
{
    if(c!=' ' && c!='\n'){
        const map<char, vector<ofPoint> >::iterator itr = pointCharMap.find(c);
        if(itr != pointCharMap.end()){
            vector<ofPoint> &point = itr->second;
            int n = point.size();
            for(int i=0; i<n; i++){
                picto * p = new picto();
                //p->setTarget(point[i]);
                pcon.push_back(p);
                p->setParent(this);
            }
            
            makeAnimationThread(point, 4);
        }
    }
    
}

void pictoChar::initAlphabetFromFontdata(){
    string fontName = "HelveticaNeueCondensedBold.ttf";
    
    string alphabet;    //= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!\"#$%'()~|@`{}+-*=/<>?_.,[]";
    for(int i=32; i<128; i++){
        alphabet += (char)i;
    }

    fontSizeDefualt = 120 *2;
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

void pictoChar::setTarget(vector<ofPoint> ps, bool _randomWalk){
    int size = min(ps.size(), pcon.size());
    
    for(int i=0; i<size; i++){
        pcon[i]->setTarget(ps[i], ofRandom(10, 30));
    }
    
    bRandomWalk = _randomWalk;
}

void pictoChar::makeAnimationThread(vector<ofPoint> ps, int numMovement){
    boost::thread t(boost::bind(&pictoChar::setAnimationChain, this, ps, numMovement));
//    t.start_thread();
}

void pictoChar::setAnimationChain(vector<ofPoint> ps1, int numMovement){

    int pointNum = ps1.size();
    int w = ofGetWidth()*0.2;
    int h = ofGetHeight();
    
    boost::asio::io_service io;
    boost::asio::deadline_timer t1(io, boost::posix_time::milliseconds(15000));
    t1.async_wait(boost::bind(&pictoChar::setTarget, this, ps1, false));

    vector<ofPoint> ps2, ps3;
    ofPoint rand1 = ofPoint(ofRandom(-w, w), ofRandom(0, h), 0);
    ofPoint rand2 = ofPoint(ofRandom(-w, w), ofRandom(h*0.3, h), 0);
    ofPoint rand3;
    for (int i=0; i<pointNum; i++) {
        int r = ofRandom(-120, 120);
        rand3 = ofPoint(ofRandom(-r,r), ofRandom(-r, r), 0);
        ps2.push_back(rand1 + rand3);
        ps3.push_back(rand2 + rand3);
    }
    
    setTarget(ps2);

    boost::asio::deadline_timer t2(io, boost::posix_time::milliseconds(1000));
    t2.async_wait(boost::bind(&pictoChar::setTarget, this, ps2, true));

    boost::asio::deadline_timer t3(io, boost::posix_time::milliseconds(5000));
    t3.async_wait(boost::bind(&pictoChar::setTarget, this, ps3, true));

//    boost::asio::deadline_timer t4(io, boost::posix_time::milliseconds(6000));
//    t4.async_wait(boost::bind(&pictoChar::setTarget, this, ps2, true));

    boost::asio::deadline_timer t5(io, boost::posix_time::milliseconds(8000));
    t5.async_wait(boost::bind(&pictoChar::setTarget, this, ps1, true));

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