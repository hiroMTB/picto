//
//  picto.cpp
//  pictoTest_rev01
//
//  Created by matobahiroshi on 13/07/03.
//
//

#include "testApp.h"
#include "picto.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>
using namespace boost;

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

picto::picto(){
    ofPoint rand = ofPoint( ofRandom(50, 400), ofRandom(400,850), 0);
    target.set(rand);
}

picto::~picto(){
    
}


void picto::setRandom(){
    ofVec3f randp(ofRandom(testApp::getW()*0.3, testApp::getW()*0.5), ofRandom(testApp::getH()*0.2, testApp::getH()*0.7), 0);
    pos.set(randp + parent->getCharPos());
    
    colorType = (int)ofRandom(0, colorTypeNum);
    imgType = (int)ofRandom(0, imgTypeNum);
    
    vel.set(0, 0);
    acc.set(0, 0);
    
    spLength = ofRandom(10, 30);
    topSpeed = ofRandom(50, 100) * (SPEED+1);
    minSpeed = ofRandom(6, 13);
    K = ofRandom(6, 20) * (ACCEL+1);
    
    if(ofRandom(0, 100)>95){
        minSpeed = ofRandom(30, 40);
    }
    
    alpha = ofRandom(1, 10);
    
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
    
    springSim();
    speedControl();
    
    
    if(parent->getClearance()){
//        alpha *= 0.95;
    }
}

void picto::springSim(){
    float dt = 0.0167;
    
    ofVec3f dir = target - pos;

    ofVec3f accel = 0.01 * K * dt * dir;
    
    if(!parent->getRandomWalk()){
        vel = vel*0.99 + (10 * accel)*0.5;
        
        if(vel.length()<minSpeed){
            vel = vel.getNormalized() * minSpeed;
            float grav = 100.0 / dir.lengthSquared();
            vel += grav;
        }
    }else{
        vel += accel;
    }

    pos += vel * dt;
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
    
    ofVec3f &home = target;
    
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

void picto::setNewTarget(ofPoint p, int time){
//        Tweener.addTween(pos.x, p.x, time, &ofxTransitions::easeOutElastic);
//        Tweener.addTween(pos.y, p.y, time, &ofxTransitions::easeOutElastic);
    newTarget = p;
}













////////////////////////////////////////////////////////
//                                               //
//  pictoChar class                              //
//                                               //
///////////////////////////////////////////////////////


std::map<char, vector<ofPoint> > pictoChar::pointCharMap;

ofTrueTypeFont pictoChar::font;
float   pictoChar::FONT_SIZE = -123;
float   pictoChar::ICON_SIZE = -123;
float pictoChar::overlapRateDefault;
float pictoChar::stringAlphaDefault;

pictoChar::pictoChar(char _c, ofVec3f _charPos)
:c(_c),
charPos(_charPos),
bRandomWalk(true),
stringAlpha(stringAlphaDefault),
iconSize(ICON_SIZE),
bClearance(false),
clearanceCounter(0)
{
    if(c!=' ' && c!='\n'){
        float res = ICON_SIZE * overlapRateDefault;
        vector<ofPoint> points = makeCharacterPointData(c, res);
        int n = points.size();
        for(int i=0; i<n; i++){
            picto * p = new picto();
            pcon.push_back(p);
            p->setParent(this);
            p->setRandom();
            p->setScale(iconSize/(float)iconSizeOriginal);
        }
        
        finalTarget = points;
        float scale = FONT_SIZE / 500.0;
        width = font.stringWidth( ofToString(c) ) * scale;
        height = font.stringHeight(ofToString(c)) * scale;
    }
}

void pictoChar::initAlphabetFromFontdata(){
    string fontName = "HelveticaNeueCondensedBold.ttf";
    
    string alphabet;    //= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!\"#$%'()~|@`{}+-*=/<>?_.,[]";
    for(int i=32; i<128; i++){
        alphabet += (char)i;
    }
    
    overlapRateDefault = 0.5;
    stringAlphaDefault = 100.0;
    
    float res = ICON_SIZE * overlapRateDefault;
    font.loadFont("type/"+fontName, 500, true, true, true);
//    font.setLetterSpacing(1.3);
    

//    cout << "font set " << alphabet << endl;
//
//    for(int i=0; i<alphabet.size(); i++){
//        char c = alphabet.at(i);
//        
//        vector<ofPoint> points = makeCharacterPointData(c, res);
//        pointCharMap.insert( pair<char, vector<ofPoint> >(c, points) );
//    }
}


//ofFbo pictoChar::fbo;
vector<ofPoint> pictoChar::makeCharacterPointData(char c, float res){
    cout << c << " " << (int)c << "       ";

    float scale = FONT_SIZE / 500.0;
    float fw = font.stringWidth( ofToString(c) ) * scale;
//    float fh = font.stringHeight(ofToString('I')) * scale;
    float fh = font.getLineHeight() * scale;
    
    ofFbo * fbo = new ofFbo();
    {
        // make fbo
        fbo->allocate(fw, fh*1.2);
        fbo->begin();
        ofFill();
        ofSetColor(0, 0, 0);
        ofRect(0, 0, fw, fh*1.2);
        ofSetColor(255, 0, 0);
        ofScale(scale, scale);
        font.drawStringAsShapes(ofToString(c), 0, fh/scale);
        fbo->end();
    }
    
    ofPixels pix;
    ofTexture targetTex;
    pix.allocate(fw, fh, OF_PIXELS_RGBA);
    targetTex.allocate(pix);
    fbo->readToPixels(pix);
    targetTex.loadData(pix);
    
    int pw = fw;
    int ph = fh*1.2;
    
    vector<ofPoint> points;
    
    for(int sx=res/2; sx<pw; sx+=res){
        for(int sy=res/2; sy<ph; sy+=res){
            ofColor col = pix.getColor(sx, sy);
            if(col.r > 200) {
                float rand = FONT_SIZE/100.0 * scale;
                points.push_back(ofPoint(sx, sy,0));
            }
        }
    }
      
    delete fbo;
    return points;

}

void pictoChar::drawString(){
    float scale = FONT_SIZE/500.0;
    float lineHeight = font.getLineHeight();
    ofSetColor(0, 0, 0, stringAlpha); //*=0.99);
    ofNoFill();
    glPushMatrix();
    glScalef(scale, scale, scale);
    font.drawStringAsShapes(ofToString(c), 0, lineHeight*scale);
    glPopMatrix();
}


void pictoChar::update(){
    
    updateAnimations();
    
    PITR itr = pcon.begin();
    for(; itr!=pcon.end(); itr++){
        (*itr)->update();
    }
}

bool pictoChar::clearanceCheck(){
    if(bClearance){
        if(++clearanceCounter > 60 * 10){
            return true;
        }
    }
    return false;
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

void pictoChar::setFinalTarget(bool _randomWalk){
    int size = finalTarget.size();
    
    for(int i=0; i<size; i++){
        pcon[i]->setNewTarget(charPos + finalTarget[i]);
    }
    
    bRandomWalk = _randomWalk;
}

//
//  rw = random width
//
void pictoChar::setTargetAround(ofPoint p, float rw, float rh, bool randomWalk, bool globalPos){
    int size = pcon.size();
    
    for(int i=0; i<size; i++){
        
        ofPoint target = p + ofPoint(ofRandom(-rw,rw), ofRandom(-rh, rh));
        if(globalPos){
            pcon[i]->setNewTarget(target);
        }else{
            pcon[i]->setNewTarget(charPos + target);
        }
    }
    
    bRandomWalk = randomWalk;
}



void pictoChar::setRandomAnimation(int milliseconds, ofPoint p, float rw, float rh, bool randomWalk, bool globalPos){
    boost::posix_time::ptime syst= boost::posix_time::microsec_clock::local_time() + boost::posix_time::milliseconds(milliseconds);
    boost::posix_time::time_duration exeTimed = syst - testApp::appStartTime;
    long exeTime = exeTimed.total_milliseconds();

    Animation a(Animation::random, exeTime, p, rw, rh, randomWalk, globalPos);
    animations.insert(pair<int, Animation>(exeTime, a));

//    printf("Add Animation time=%i, Atype= random\n", exeTime);

}

void pictoChar::setFinalAnimation(int milliseconds, bool randomWalk){
    boost::posix_time::ptime syst= boost::posix_time::microsec_clock::local_time() + boost::posix_time::milliseconds(milliseconds);
    boost::posix_time::time_duration exeTimed = syst - testApp::appStartTime;
    long exeTime = exeTimed.total_milliseconds();
    Animation a(Animation::target, exeTime, ofPoint(0,0), 0, 0, randomWalk);
    animations.insert(AnimationData(exeTime, a));
    
//    printf("Add Animation time=%i, Atype= Final\n", exeTime);
    
}

void pictoChar::setEndAnimation(int milliseconds, bool randomWalk){
    boost::posix_time::ptime syst= boost::posix_time::microsec_clock::local_time() + boost::posix_time::milliseconds(milliseconds);
    boost::posix_time::time_duration exeTimed = syst - testApp::appStartTime;
    long exeTime = exeTimed.total_milliseconds();

    Animation a(Animation::end, exeTime, ofPoint(testApp::getW()*10,testApp::getH()/2), testApp::getH()*0.2, testApp::getH()*0.6, randomWalk);
    animations.insert(AnimationData(exeTime, a));
    
//    printf("Add Animation time=%i, Atype= END \n", exeTime);

//    int deathtime = milliseconds + 3*1000;
//    Animation death(Animation::death, deathtime, ofPoint(0,0),0,0,false);
//    animations.insert(AnimationData(deathtime, death));
}




void pictoChar::clearAnimation(int milliseconds){

    setEndAnimation(milliseconds, true);
    bClearance = true;

    //    ofPoint screenOut(testApp::getW()*2, testApp::getH()*0.5);
//    int range = testApp::getH() * 0.3;
//    setRandomAnimation(waitTime, screenOut, range, range, true);
    
}

pictoChar::~pictoChar(){
//    testApp::getInstance()->clearFromPictoString(this);

    PITR itr = pcon.begin();
    for(; itr!=pcon.end(); itr++){
        delete (*itr);
        (*itr) = 0;
    }
    pcon.clear();
    
}


void pictoChar::updateAnimations()
{
    boost::posix_time::time_duration pnowd = boost::posix_time::microsec_clock::local_time() - testApp::appStartTime;
    long nowd = pnowd.total_milliseconds();
//    printf("NOW is %i\n", nowd);
    
    bool shouldClearContainer = false;
    
    AnimationContItr itr = animations.begin();
    for(; itr!=animations.end(); itr++){
        Animation& a = itr->second;
        long deadline = a.exeTime;
        
        if(!a.done && deadline < nowd){
            a.done = true;
            
            switch(a.type){
                case Animation::random:
                    setTargetAround(a.randPoint, a.rw, a.rh, a.randomWalk, true);
//                    printf("RANDOM TARGET\n");
                    break;
                case Animation::target:
                    setFinalTarget(a.randomWalk);
//                    printf("FINAL TARGET\n");
                    break;
                case Animation::end:
                    setTargetAround(a.randPoint, a.rw, a.rh, a.randomWalk, true);
                    shouldClearContainer = true;
//                    printf("END\n");

                    break;
                    
                default:
                    break;
            }
        }
    }
    
    if(shouldClearContainer)
        animations.clear();
    
}

