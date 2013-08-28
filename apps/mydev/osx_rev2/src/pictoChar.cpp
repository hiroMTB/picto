////////////////////////////////////////////////////////
//                                               //
//  pictoChar class                              //
//                                               //
///////////////////////////////////////////////////////

#include "testApp.h"
#include "pictoChar.h"

float pictoChar::ICON_DISTANCE = -123;
float pictoChar::FONT_RANDOMNESS = -123;
float pictoChar::LINE_HEIGHT = -123;
float pictoChar::LETTER_SPACING = -123;
float pictoChar::FONT_SIZE = -123;
float pictoChar::ICON_SIZE = -123;
float pictoChar::stringAlphaDefault;

std::map<char, vector<ofVec2f> > pictoChar::pointCharMap;
ofTrueTypeFont pictoChar::font;

pictoChar::pictoChar(char _c, ofVec2f _charPos, pictoString * _parent)
:c(_c),
charPos(_charPos),
parent(_parent),
bRandomWalk(true),
stringAlpha(stringAlphaDefault),
iconSize(ICON_SIZE),
bClearance(false),
clearanceCounter(0)
{

    if(c!=' ' && c!='\n'){
        float res = ICON_SIZE * ICON_DISTANCE;
        vector<ofVec2f> points = makeCharacterPointData(c, res);
        int n = points.size();
        for(int i=0; i<n; i++){
            picto * p = new picto();
            pcon.push_back(p);
            p->setParent(this);
            p->setRandom();
            p->setScale(iconSize/(float)picto::getIconSizeOriginal());
        }
        
        finalTarget = points;
        float scale = getFontScale();
        width = font.stringWidth( ofToString(c) ) * scale;
        height = font.stringHeight(ofToString(c)) * scale;
    }
}

void pictoChar::init(){
    string fontName = "HelveticaNeueCondensedBold.ttf";
    stringAlphaDefault = 100.0;
    font.loadFont("type/"+fontName, 500, true, true, true);
}

vector<ofVec2f> pictoChar::makeCharacterPointData(char c, float res){
    
    font.setLetterSpacing(LETTER_SPACING);
    font.setLineHeight(LINE_HEIGHT);
    
    float scale = getFontScale();
    float fw = font.stringWidth( ofToString(c) ) * scale;
    float fh = font.getLineHeight() * scale;
    
    ofFbo * fbo = new ofFbo();
    {
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
    pix.allocate(fw, fh*1.2, OF_PIXELS_RGBA);
    targetTex.allocate(pix);
    fbo->readToPixels(pix);
    targetTex.loadData(pix);
    
    int pw = fw;
    int ph = fh*1.2;
    vector<ofVec2f> points;
    
    for(int sx=res/2; sx<pw; sx+=res){
        for(int sy=res/2; sy<ph; sy+=res){
            ofColor col = pix.getColor(sx, sy);
            if(col.r > 200) {
                float rand = FONT_SIZE/100.0 * scale * FONT_RANDOMNESS;
                ofVec2f randp(ofRandom(-rand, rand), ofRandom(-rand, rand));
                points.push_back(ofVec2f(sx, sy) + randp);
            }
        }
    }
    
    delete fbo;
    return points;
}

void pictoChar::update(){
    updateAnimations();
    
    PITR itr = pcon.begin();
    for(; itr!=pcon.end(); itr++){
        picto * p = (*itr);
        p->update();
        ofVec2f& pos = p->getPos();
    }
}



void pictoChar::draw(){
    ofSetRectMode(OF_RECTMODE_CENTER);
    PITR itr = pcon.begin();
    for(; itr!=pcon.end(); itr++){
        (*itr)->draw();
    }
    
    ofSetColor(0, 244, 0);
    ofFill();
//    ofRect(cloudCenter.x, cloudCenter.y, 30, 30);
    
    ofSetRectMode(OF_RECTMODE_CORNER);
    
}

void pictoChar::drawTarget(){
    PITR itr = pcon.begin();
    for(; itr!=pcon.end(); itr++){
        (*itr)->drawTarget();
    }
}

void pictoChar::drawString(){
    float scale = getFontScale();
    float lineHeight = font.getLineHeight();
    ofSetColor(0, 0, 0, stringAlpha); //*=0.99);
    ofNoFill();
    glPushMatrix();
    glScalef(scale, scale, scale);
    font.drawStringAsShapes(ofToString(c), 0, lineHeight*scale);
    glPopMatrix();
}

bool pictoChar::clearanceCheck(){
    if(bClearance){
        if(++clearanceCounter > 60 * 10){
            return true;
        }
    }
    return false;
}

void pictoChar::drawFontText(string s, int x, int y){
    float scale = getFontScale();
    float fh = font.getLineHeight();
    
    glPushMatrix();
    glScalef(scale, scale, scale);
    font.drawString(s, x, y+fh);
    glPopMatrix();
}

void pictoChar::setFinalTarget(bool _randomWalk){
    int size = finalTarget.size();
    
    for(int i=0; i<size; i++){
        pcon[i]->setNewTarget(charPos + finalTarget[i]);
    }
    
    bRandomWalk = _randomWalk;
}


//
//
//      OMOI !!!!!!!
//
//
void pictoChar::setTargetAround(ofVec2f& p, float rw, float rh, bool randomWalk, bool globalPos){
    int size = pcon.size();
    
    for(int i=0; i<size; i++){
        
        ofVec2f target = p + ofVec2f(ofRandom(-rw,rw), ofRandom(-rh, rh));
        if(globalPos){
            pcon[i]->setNewTarget(target);
        }else{
            pcon[i]->setNewTarget(charPos + target);
        }
    }
    
    bRandomWalk = randomWalk;
}

void pictoChar::printAnimationList(){
    int now = ofGetFrameNum();

    printf("\n AnimationList now= %i\n", now);
    AnimationContItr itr = animations.begin();
    for(int i=0; itr!=animations.end(); itr++, i++){
        Animation * a = itr->second;
        int deadline = a->exeFrame;

        printf("A# %i type: %i, dead: %i\n", i+1, a->type, deadline);
    }
    
}

void pictoChar::updateAnimations(){
    //    long now = testApp::getNow();
    int now = ofGetFrameNum();
    
    bool shouldClearContainer = false;
    
    AnimationContItr itr = animations.begin();
    for(; itr!=animations.end(); itr++){
        Animation * a = itr->second;
        int deadline = a->exeFrame;
        
        if(!a->done && deadline < now){
            a->done = true;
            
            switch(a->type){
                case Animation::random:
                    setTargetAround(a->randPoint, a->rw, a->rh, a->randomWalk, true);
                    break;
                case Animation::target:
                    setFinalTarget(a->randomWalk);
                    break;
                case Animation::end:
                    setTargetAround(a->randPoint, a->rw, a->rh, a->randomWalk, true);
                    shouldClearContainer = true;
                    break;
                    
                default: break;
            }
        }
    }
    
    if(shouldClearContainer){
        itr = animations.begin();
        for(; itr!=animations.end(); itr++){
            Animation * a = itr->second;
            delete a;
            a = 0;
        }
        animations.clear();
    }
    
}

void pictoChar::setRandomAnimation(int milliseconds, ofVec2f& p, float rw, float rh, bool randomWalk, bool globalPos){
    int exeFrame = ofGetFrameNum()+ milliseconds/1000.0*60.0;
    Animation * a = new Animation(Animation::random, exeFrame, p, rw, rh, randomWalk, globalPos);
    animations.insert(AnimationData(exeFrame, a));
//    printf("Random Animation at Frame: %i\n", exeFrame);
}

void pictoChar::setFinalAnimation(int milliseconds, bool randomWalk){
    int exeFrame = ofGetFrameNum()+ milliseconds/1000.0*60.0;
    Animation * a = new Animation(Animation::target, exeFrame, ofVec2f(0,0), 0, 0, randomWalk);
    animations.insert(AnimationData(exeFrame, a));
//    printf("Final Animation at Frame: %i\n", exeFrame);
}

void pictoChar::setEndAnimation(int milliseconds, bool randomWalk){
    int exeFrame = ofGetFrameNum()+ milliseconds/1000.0*60.0;
    Animation * a = new Animation(Animation::end, exeFrame, ofVec2f(testApp::getW()*6,testApp::getH()/2), testApp::getH()*0.2, testApp::getH()*2.2, randomWalk);
    animations.insert(AnimationData(exeFrame, a));
//    printf("End Animation at Frame: %i\n", exeFrame);
}

void pictoChar::clearAnimation(int milliseconds){
    setEndAnimation(milliseconds, true);
    bClearance = true;
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

