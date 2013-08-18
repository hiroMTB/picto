#include "testApp.h"

testApp * testApp::instance = NULL;


//--------------------------------------------------------------
void testApp::setup(){

	ofSetVerticalSync(true);
	ofSetFrameRate(60);
  	ofBackground(255);
	ofSetColor(255);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    offsetPos.set(70, 100, 0);
    
    picto::init();
    pictoChar::initAlphabetFromFontdata();
    
//    testAnimation();
    
    bDebugDraw = true;
    bCap = false;
    bShowInfo = false;
    bRealtime = false;
}

void testApp::testAnimation(){
//    pictoChar * pchar = new pictoChar('A');
//    pictoString.push_back(pchar);
}


//--------------------------------------------------------------
void testApp::update(){
    //Tweener.update();

    vector<pictoChar*>::iterator itr = pictoString.begin();
    for(; itr!=pictoString.end(); itr++){
        (*itr)->update();
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofPushMatrix();{
        glTranslatef(offsetPos.x, offsetPos.y, 0);
        ofFill();
    
        vector<pictoChar*>::iterator itr = pictoString.begin();
        for(int i=0; itr!=pictoString.end(); itr++, i++){

            ofPushMatrix();
            if(bDebugDraw) (*itr)->drawTarget();
            (*itr)->draw();
            
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
            }else{
                
                int rx = ofGetWidth() * 0.5;
                int ry = ofGetHeight() * 0.5;
                
                int w = 230;
                int h = 280;
                
                vector<ofPoint> ps1, ps2, ps3;
                vector<ofPoint> charPosList;
                
                for(int i=0; i<inputText.size(); i++){
                    char c = inputText.at(i);
                    int totalLen = i*w;
                    int x = totalLen % (ofGetWidth()-w);
                    int y = h * (int)(totalLen / (ofGetWidth()-w*0.6));

                    ofPoint charPos = ofVec3f(x, y, 0);
                    charPosList.push_back(charPos);
                    
                    pictoChar * pchar = new pictoChar(c, charPos);
                    pictoString.push_back(pchar);
                    
                    int n = pchar->getInstanceNum();
                    for(int j=0; j<n; j++){
                        ofPoint rand1 = ofPoint( ofRandom(200, 300), ofRandom(600, 700), 0);
                        ofPoint rand2 = ofPoint( ofRandom(1400, 1600), ofRandom(600, 700), 0);

                        ps1.push_back(rand1);
                        ps2.push_back(rand2);
                    }
                    
                    
                    pchar->setAnimation(ps1, 1, true);
                    pchar->setAnimation(ps2, 1000, true);

                    int d;
                    
                    for(int j=0; j<charPosList.size(); j++){
                        d = j*1000;
                        for(int k=0; k<n; k++){
                            ofPoint rand3 = ofPoint( ofRandom(-60, 60), ofRandom(30, 200), 0);
                            ps3.push_back(charPosList[j] + rand3);
                        }
                        pchar->setAnimation(ps3, 5000 + d, true);
                        ps3.clear();
                    }
                    
                    pchar->setAnimation(c,   6000 + d+1000, false);
                }
                
                inputText = "";
            }
            break;

        case OF_KEY_BACKSPACE:
        {
            if(pictoString.size()>0){
                for(int h=0; h<pictoString.size(); h++){
                    pictoChar * pchar = pictoString[h];

                    vector<ofPoint> ps;
                    int n= pchar->getInstanceNum();
                    for(int i=0; i<n; i++){
                        int x = ofRandom(ofGetWidth()*3, ofGetWidth()*3);
                        int y = ofRandom(-ofGetHeight()*0.1, ofGetHeight()*1.1);
                        ps.push_back(ofPoint(x, y, 0));
                    }
                    
                    pchar->setAnimation(ps, 30*h , true);
    //                if(pchar){
    //                    pictoString.pop_back();
    //                    delete pchar;
    //                    pchar = 0;
    //                }
                }
            }
        }
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
            if(bRealtime){
                int x, y = 0;
                int w = 240;
                int h = 330;

                int totalLen = pictoString.size()*w;
                x = totalLen % (ofGetWidth()-w);
                y = h * (int)(totalLen / (ofGetWidth()-w));
                
                pictoChar * pchar = new pictoChar(key, ofVec3f(x, y, 0));
                pictoString.push_back(pchar);
                
                
                vector<ofPoint> ps1, ps2, ps3;
                int n = pchar->getInstanceNum();
                for(int j=0; j<n; j++){
                    ofPoint rand1 = ofPoint( ofRandom(0, 500), ofRandom(500, 800), 0);
                    ofPoint rand2 = ofPoint( ofRandom(1400, 1700), ofRandom(500, 800), 0);
                    
                    ps1.push_back(rand1);
                    ps2.push_back(rand2);
                }
                
                
                pchar->setAnimation(ps1, 1, true);
                pchar->setAnimation(ps2, 1000, true);
                pchar->setAnimation(key,   3000, false);

            }else{
                inputText += key;
            }
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
//    int n = pman.getInstanceNum();
//    vector<ofPoint> ps;
//    //ps.assign(n, ofPoint(x, y));
//    for(int i=0; i<n; i++){
//        float r = ofRandom(40, 200);
//        float angle = ofRandom(-360, 360);
//        float px = x + sin(angle) * r;
//        float py = y + cos(angle) * r * 0.7;
//        
//        ps.push_back(ofPoint(px, py));
//    }
//    
//	pman.setTarget(ps);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
	
}
