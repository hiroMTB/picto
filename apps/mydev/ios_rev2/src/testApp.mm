#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	iPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);
	
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(244,245,237);
    
    inputText = "POBRB";
    scale = max(ofGetHeight(), ofGetHeight()) *0.008;
    offsetPos.set(20, ofGetHeight()*0.4, 0);
    
    
    pman.init();
    
    font.loadFont("type/DIN.otf", 100*scale, true, false, true);
    chars = font.getStringAsPoints(inputText);

    fbo.allocate((float)ofGetWidth()*.3, (float)ofGetHeight()*.3, OF_PIXELS_RGBA);
    
    float posx = 0;
    for(int i=0; i<chars.size(); i++){
        
        char c = inputText.at(i);
        
//        ofPoint center = chars[i].get
        
            float fw = font.stringWidth( ofToString(c) )*1.2;
            float fh = font.stringHeight(ofToString(c))*1.2;
        
        {
            // make fbo
            fbo.allocate(fw, fh);
            fbo.begin();
            ofSetColor(0, 0, 0);
            ofRect(0, 0, fbo.getWidth(), fbo.getHeight());
            
            ofSetColor(255, 0, 0);
            ofRectangle bounds = font.getStringBoundingBox( ofToString(c), 0, 0);
            font.drawStringAsShapes(ofToString(c), -bounds.x, -bounds.y);
            fbo.end();
        }
        
        // here we need some trick
        // see this http://forum.openframeworks.cc/index.php?topic=7392.0
        ofPixels pix;
		ofTexture targetTex;
        pix.allocate(fbo.getTextureReference().getWidth(),fbo.getTextureReference().getHeight(), OF_PIXELS_RGBA);
        targetTex.allocate(pix);
        fbo.readToPixels(pix);
        targetTex.loadData(pix);
        

        int res = 22; // pix
        int row = fw / res;
        int col = fh / res;
        
        for(int j=0; j<row; j++){
            for(int k=0; k<col; k++){
                int x = res * j;
                int y = res * k;
                ofColor col = pix.getColor(x, y);
                if(col.r > 200) {
                    
                    picto * p = new picto();
                    p->setPos(ofPoint(ofRandom(0, ofGetWidth())-offsetPos.x, ofRandom(0, ofGetHeight())-offsetPos.y, 0));
                    p->setScale(scale);
                    p->setAngle(ofRandom(-30, 30));
                    
                    int randomize = res/3;
                    p->setTarget( ofPoint(posx,0,0)+ofPoint(x,y,0), ofRandom(1, 10));
                    pman.add(p, ofRandom(2));
                }
            }
        }
        
        posx += fw + 10;
    }
    
//    cap.initGrabber(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void testApp::update(){


//    cap.update();
    Tweener.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    
	ofPushMatrix();
    //glTranslatef(ofGetWidth()/2, ofGetHeight()/2, 0);
    glTranslatef(offsetPos.x, offsetPos.y, 0);
    pman.draw();
    ofPopMatrix();
    
    ofSetColor(0);
    ofRect(5, 5, 200, 60);
  	ofSetColor(255);
    ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()),20,20);
    ofDrawBitmapString("svg num: " + ofToString(pman.getInstanceNum()),20,40);
    
    //fbo.draw(100, 100);
}

//--------------------------------------------------------------
void testApp::exit(){

}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void testApp::lostFocus(){

}

//--------------------------------------------------------------
void testApp::gotFocus(){

}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){

}

