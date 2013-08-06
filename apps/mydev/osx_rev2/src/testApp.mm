#include "testApp.h"

testApp * testApp::instance = NULL;



//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
  	ofBackground(255);
	ofSetColor(255);
    
    offsetPos.set(50, 60, 0);
    
    picto::init();
    pictoChar::initAlphabetFromFontdata();
    
    testAnimation();
    
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    bDebugDraw = true;
    bCap = false;
    bShowInfo = false;
}

void testApp::testAnimation(){
    
    pictoChar * pchar = new pictoChar('A');
    pictoString.push_back(pchar);
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
    
        float x, y = 0;
        float w = 270;
        float h = 330;
        vector<pictoChar*>::iterator itr = pictoString.begin();
        for(int i=0; itr!=pictoString.end(); itr++, i++){

            ofPushMatrix();
            glTranslatef(x, y, 0);
            
            if(bDebugDraw) (*itr)->drawTarget();
            (*itr)->draw();
            
            //glTranslatef(0, h, 0);
            //(*itr)->drawString();
            glPopMatrix();
            
            x+=w;
            if(x>ofGetWidth()-w){
                x = 0;
                y += h;
            }

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
            pictoString.clear();
            break;

        case OF_KEY_BACKSPACE:
        {
            if(pictoString.size()>0){
                pictoChar * pchar = pictoString.back();

                if(pchar){
                    pictoString.pop_back();
                    delete pchar;
                    pchar = 0;
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
    
        case 'f': ofToggleFullscreen();
        case 'b': bDebugDraw = !bDebugDraw;
        case 'i': bShowInfo = !bShowInfo;
            
        default:
            pictoChar * pchar = new pictoChar(key);
            pictoString.push_back(pchar);
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
