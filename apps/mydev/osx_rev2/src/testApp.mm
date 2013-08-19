#include "testApp.h"

testApp * testApp::instance = NULL;

bool testApp::bBlack = false;
bool testApp::bDebugDraw = false;
ofColor testApp::bg = ofColor(0,0,0);

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
        glTranslatef(offsetPos.x, offsetPos.y, 0);
        ofFill();
    
        vector<pictoChar*>::iterator itr = pictoString.begin();
        for(int i=0; itr!=pictoString.end(); itr++, i++){

            ofPushMatrix();
            if(bDebugDraw) (*itr)->drawTarget();
            (*itr)->draw();
//            (*itr)->drawString();
            glPopMatrix();
        }
    
    }ofPopMatrix();
    
    if(bShowInfo){
        ofPushMatrix();
        ofTranslate(0,0);
        ofSetColor(0);
        ofFill();
        ofRect(0, 0, 200, 200);
        ofSetColor(255);
        ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()),20,20);
        //ofDrawBitmapString("icon num: " + ofToString(pman.getInstanceNum()),20,40);
        ofDrawBitmapString("inputText.length: " + ofToString(pictoString.size()),20,60);
        //        ofDrawBitmapString("iconSize: " + ofToString(iconSize) + " pixel",20,80);
        //        ofDrawBitmapString("fontSize: " + ofToString(fontSize) + " pixel",20,100);
        
        ofPopMatrix();
    }

    if(bCap){
        ofImage image;
        image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        image.saveImage("shot.png");
        bCap = false;
    }


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
void testApp::windowResized(int w, int h){}
void testApp::gotMessage(ofMessage msg){}
void testApp::dragEvent(ofDragInfo dragInfo){}


void testApp::makeAnimation(string s){
    
    int ww = ofGetWidth();
    int wh = ofGetHeight();
    
    int rx = ww * 0.5;
    int ry = wh * 0.5;
    
    int posx = 30;
    int posy = 30;
    int spacex = 45;
    int spacey = 45;
    int maxHeight = 0;
    int maxWidth = 0;
    
    vector<ofPoint> ps1, ps2, ps3;
    vector<ofPoint> charPosList;
    
    for(int i=0; i<s.size(); i++){
        char c = s.at(i);
        
        if(posx > ofGetWidth() - maxWidth){
            posx = 30;
            posy += maxHeight+spacey;
            maxHeight = 0;
        }
        
        ofPoint charPos = ofVec3f(posx, posy, 0);
        charPosList.push_back(charPos);
        
        pictoChar * pchar = new pictoChar(c, charPos);
        posx += pchar->getWidth()+spacex;
        maxHeight = MAX(maxHeight, pchar->getHeight());
        maxWidth = MAX(maxWidth, pchar->getWidth());

        
        pictoString.push_back(pchar);
        
        int n = pchar->getInstanceNum();
        
        ofPoint rand1 = ofPoint( ofRandom(ww*0.1, ww*0.15), ofRandom(wh*0.55, wh*0.64), 0);
        ofPoint rand2 = ofPoint( ofRandom(ww*0.72, ww*0.83), ofRandom(wh*0.55, wh*0.64), 0);
        
        float range = ofGetWidth()*0.1;
        pchar->setRandomAnimation(rand1, range, range, 1, true);
        pchar->setRandomAnimation(rand2, range, range, 1000, true);
        
        int d = 0;
        
//        for(int j=0; j<charPosList.size(); j++){
//            d = j*1000;
//            pchar->setRandomAnimation(charPosList[j], 60, 100, 5000+d, true);
//        }
        
        pchar->setFinalAnimation(6000+d+1000, false, false);
    }
}


void testApp::clearAll(){
    if(pictoString.size()>0){
        for(int h=0; h<pictoString.size(); h++){
            pictoChar * pchar = pictoString[h];
            pchar->clearAnimation(h*30);
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


