#include "testApp.h"


boost::posix_time::ptime testApp::appStartTime = boost::posix_time::microsec_clock::local_time();

testApp * testApp::instance = NULL;

bool testApp::bBlack = false;
bool testApp::bDebugDraw = false;
ofColor testApp::bg = ofColor(0,0,0);

float testApp::w = 0;
float testApp::h = 0;

//--------------------------------------------------------------
void testApp::setup(){

	ofSetVerticalSync(true);
	ofSetFrameRate(60);
  	ofBackground(255);
	ofSetColor(255);

    
    offsetPos.set(0, 0, 0);
    
    picto::init();
    pictoChar::initAlphabetFromFontdata();
    
    bCap = false;
    bShowInfo = false;
    bRealtime = false;
}


//--------------------------------------------------------------
void testApp::update(){
    
    vector<pictoChar*>::iterator itr = pictoString.begin();
    vector<pictoChar*> remover;
    for(; itr!=pictoString.end(); itr++){
        (*itr)->update();
        if((*itr)->clearanceCheck()){
            remover.push_back(*itr);
        }
    }

    vector<pictoChar*>::iterator itr2;
    for(int i=0; i<remover.size(); i++){
        clearFromPictoString(remover[i]);
    }
    
}


//--------------------------------------------------------------
void testApp::draw(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    if(bBlack){
        ofBackground(0,0,0);
        return;
    }
    
    ofBackground(bg);
    
    ofPushMatrix();{
        ofFill();
    
        vector<pictoChar*>::iterator itr = pictoString.begin();
        for(int i=0; itr!=pictoString.end(); itr++, i++){

            ofPushMatrix();
            if(bDebugDraw) (*itr)->drawTarget();
//            (*itr)->drawFbo();
            (*itr)->draw();
            glPopMatrix();
        }
    
    }ofPopMatrix();
    
    if(bDebugDraw){
        ofPushMatrix();
        ofTranslate(0,0);
        ofSetColor(0);
        ofFill();
        ofRect(0, 0, w, 30);
        ofSetColor(255);
        int y = 23;
        ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()),20,y);
        ofDrawBitmapString("inputText.length: " + ofToString(pictoString.size()), 200, y);
        
        boost::posix_time::time_duration pnowd = boost::posix_time::microsec_clock::local_time() - testApp::appStartTime;
        long nowd = pnowd.total_milliseconds();
        ofDrawBitmapString("now: " + ofToString(nowd), 400, y);
        
        ofPopMatrix();
    }

    if(bCap){
        ofImage image;
        image.grabScreen(0, 0, w, h);
        image.saveImage("shot.png");
        bCap = false;
    }


}

void testApp::drawPreviewView(string s){
//    ofEnableAlphaBlending();
//    ofEnableSmoothing();
    ofTrueTypeFont& font = pictoChar::getFont();
    
    float scalex = (float)(305.0/(float)w);
    float scaley = (float)(183.0/(float)h);
    ofBackground(255);

    ofPushMatrix();{
        glScalef(scalex, scaley, 1);
        ofNoFill();
        ofSetColor(0,0,0);
        
        int posx = offsetPos.x;
        int posy = offsetPos.y;
        float fontScale = pictoChar::getFontScale();
        float lineHeightScaled = font.getLineHeight()* fontScale;

//            pictoChar::drawFontText(previewText, 0, 0);

        for(int i=0; i<previewText.size(); i++){
            char c = previewText.at(i);
            float charw = font.getCharProps(c).setWidth * fontScale;
            
            if(c == '\n'){
                posx = offsetPos.x;
                posy += lineHeightScaled;
                continue;
            }
            
            if(posx > w - charw){
                posx = offsetPos.x;
                posy += lineHeightScaled;
            }
            
            glPushMatrix();
            glTranslatef(posx, posy, 0);
            pictoChar::drawFontText(ofToString(c), 0, 0);
            glPopMatrix();
            
            posx += charw;
        }
        
    }ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key){
        case OF_KEY_RETURN:
            if(bRealtime){
                pictoString.clear();
            }
            break;
        break;
            
        case 'C':
//        {
//            bCap = true;
//            
//        }
            break;
    
        case 'f': ofToggleFullscreen(); break;
        case 'b': bDebugDraw = !bDebugDraw; break;
        case 'i': bShowInfo = !bShowInfo; break;
        
        default:
//            if(bRealtime){
//                int x, y = 0;
//                int w = 240;
//                int h = 330;
//
//                int totalLen = pictoString.size()*w;
//                x = totalLen % (ofGetWidth()-w);
//                y = h * (int)(totalLen / (ofGetWidth()-w));
//                
//                pictoChar * pchar = new pictoChar(key, ofVec3f(x, y, 0));
//                pictoString.push_back(pchar);
//                
//                
//                vector<ofPoint> ps1, ps2, ps3;
//                int n = pchar->getInstanceNum();
//                for(int j=0; j<n; j++){
//                    ofPoint rand1 = ofPoint( ofRandom(0, 500), ofRandom(500, 800), 0);
//                    ofPoint rand2 = ofPoint( ofRandom(1400, 1700), ofRandom(500, 800), 0);
//                    
//                    ps1.push_back(rand1);
//                    ps2.push_back(rand2);
//                }
//                
//                
//                pchar->setAnimation(ps1, 1, true);
//                pchar->setAnimation(ps2, 1000, true);
//                pchar->setAnimation(key,   3000, false);
//
//            }else{
//                inputText += key;
//            }
            break;
    }
}

void testApp::keyReleased(int key){}
void testApp::mouseMoved(int x, int y){}
void testApp::mouseDragged(int x, int y, int button){}
void testApp::mousePressed(int x, int y, int button){}
void testApp::mouseReleased(int x, int y, int button){}
void testApp::windowResized(int _w, int _h){
    w = _w;
    h = _h;
}
void testApp::gotMessage(ofMessage msg){}
void testApp::dragEvent(ofDragInfo dragInfo){}


void testApp::makeAnimation(string s){
    
    ofTrueTypeFont& font = pictoChar::getFont();
    float fontScale = pictoChar::getFontScale();
    
    int rx = w * 0.5;
    int ry = h * 0.5;
    
    int offsetx = offsetPos.x;
    int offsety = offsetPos.y;
    int posx = offsetx;
    int posy = offsety;
    int spacex = 45;
    int spacey = 45;
    float lineHeight = font.getLineHeight() * fontScale;
    vector<ofPoint> ps1, ps2, ps3;
    vector<ofPoint> charPosList;
    
    for(int i=0; i<s.size(); i++){
        char c = s.at(i);
        float charw = font.getCharProps(c).setWidth * fontScale;
        
        if(c == '\n'){
            posx = offsetx;
            posy += lineHeight;
            continue;
        }
        
        if(posx > w - charw){
            posx = offsetx;
            posy += lineHeight;
        }
        
        ofPoint charPos = ofVec3f(posx, posy, 0);
        charPosList.push_back(charPos);
        
        pictoChar * pchar = new pictoChar(c, charPos);
        posx += charw;
        
        
        pictoString.push_back(pchar);
        
        int n = pchar->getInstanceNum();
        
        ofPoint rand1 = ofPoint( ofRandom(w*0.1, w*0.13), ofRandom(h*0.55, h*0.60), 0);
        ofPoint rand2 = ofPoint( ofRandom(w*0.76, w*0.82), ofRandom(h*0.55, h*0.60), 0);
        
        float range = w*0.1;

        int time = 1;
        pchar->setRandomAnimation(time, rand1, range, range, true);
        time += 1000;
        pchar->setRandomAnimation(time, rand2, range, range, true);
        time += 3000;
        
        for(int j=0; j<charPosList.size(); j++){
            time += 200;
            pchar->setRandomAnimation(time, charPosList[j], 60, 100, true);
        }
        
        time += 3000;
        pchar->setFinalAnimation(time, false);
    }
}


void testApp::clearAll(){
    if(pictoString.size()>0){
        for(int h=0; h<pictoString.size(); h++){
            pictoChar * pchar = pictoString[h];
            pchar->clearAnimation(h*50);
        }
    }
}


void testApp::clearFromPictoString(pictoChar *c){
    vector<pictoChar*>::iterator itr;
    itr = std::find(pictoString.begin(), pictoString.end(), c);
    if(itr != pictoString.end()){
        pictoString.erase(itr);
    }
}



//
//      screen settings
//
void testApp::setBlack(bool b){
    bBlack = b;
}

void testApp::setFullscreen(bool b){
    ofSetFullscreen(b);
}

void testApp::setBackgroundColor(int r, int g, int b){
    bg.set(r,g,b);
}

void testApp::setDebugDraw(bool b){
    bDebugDraw = b;
}


