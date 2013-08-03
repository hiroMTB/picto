#include "testApp.h"

testApp * testApp::instance = NULL;



//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
  	ofBackground(255);
	ofSetColor(255);
    
    offsetPos.set(50, fontSize+130, 0);
    
    picto::init();
    pictoChar::initAlphabetFromFontdata();
    
//    initAnimation(inputText);
    testAnimation();
    
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    bDebugDraw = true;
    bCap = false;
    
}

void testApp::testAnimation(){
    
    pictoChar * pchar = new pictoChar('A');
    pictoString.push_back(pchar);
}

void testApp::initAnimation(string text){

//    //font.loadFont("type/cooperBlack.ttf", fontSize, true, false, true);
//    font.loadFont("type/HelveticaNeueCondensedBold.ttf", fontSize, true, false, true);
//
//
//    float spaceSize = font.getSpaceSize();
//    float letterSpacing = font.getLetterSpacing();
//    float lineHeight = font.getLineHeight();
//    
//    float posx = 0;
//    float posy = 0;
//    for(int i=0; i<text.size(); i++){
//        char c = text.at(i);
//        
//        if(c == '\n'){
//            posy += lineHeight;
//            posx = 0;
//            continue;
//        }
//                
//        float fw = font.stringWidth( ofToString(c) );
//        float fh = font.stringHeight(ofToString(c));
//        
//        ofFbo * fbo = new ofFbo();
//        fbos.push_back(fbo);
//        
//        {
//            // make fbo
//            fbo->allocate(fw, fh);
//            fbo->begin();
//            ofSetColor(0, 0, 0);
//            ofRect(0, 0, fbo->getWidth(), fbo->getHeight());
//            
//            ofSetColor(255, 0, 0);
//            ofRectangle bounds = font.getStringBoundingBox( ofToString(c), 0, 0);
//            font.drawStringAsShapes(ofToString(c), -bounds.x, -bounds.y);
//            fbo->end();
//        }
//        
//        // here we need some trick
//        // see this http://forum.openframeworks.cc/index.php?topic=7392.0
//        ofPixels pix;
//		ofTexture targetTex;
//        pix.allocate(fbo->getTextureReference().getWidth(),fbo->getTextureReference().getHeight(), OF_PIXELS_RGBA);
//        targetTex.allocate(pix);
//        fbo->readToPixels(pix);
//        targetTex.loadData(pix);
//        
//        int pw = pix.getWidth();
//        int ph = pix.getHeight();
//        float res = iconSize * overlapRate;
//
////        if(posx > ofGetWidth()){
////            posy += ph * 1.4;
////            posx = 0;
////        }
//        
//      
//        for(int sx=res/2; sx<pw; sx+=res){
//            for(int sy=res/2; sy<ph; sy+=res){
//                ofColor col = pix.getColor(sx, sy);
//                if(col.r > 200) {
//                    picto * p = new picto();
//                    p->setPos(ofPoint(ofRandom(ofGetWidth()*0.8, ofGetWidth()*1.6)-offsetPos.x, ofRandom(-ofGetHeight()*0.3, ofGetHeight()*1.3)-offsetPos.y, 0));
//                    p->setScale(iconSize / originalSvgIconSize);
//                    p->setAngle(ofRandom(-30, 30));
////                    p->setColorType(ofRandom(0,5));
//                    float rand = iconSize/10;
//                    p->setTarget( ofPoint(posx,posy,0)+ofPoint(sx,sy-fh,0) + ofPoint(ofRandom(-rand, rand), ofRandom(-rand, rand)), ofRandom(8, 40));
//                    pman.add(p, ofRandom(2));
//                }
//            }
//        }
//        
//        posx += fw * spaceSize * letterSpacing + 60;
//    }

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
    
//    ofPushMatrix();
//    glTranslatef(0,0,0);
//    ofScale(10, 10);
//    pman.drawOriginalPicto(0);
//    
//    ofPopMatrix();
    
    ofPushMatrix();
    
    glTranslatef(offsetPos.x, offsetPos.y, 0);
    
        // picto icon
        ofPushMatrix();
        ofFill();
    
        float x, y = 0;
        vector<pictoChar*>::iterator itr = pictoString.begin();
        for(int i=0; itr!=pictoString.end(); itr++, i++){

            ofPushMatrix();
            glTranslatef(x, y, 0);
            
            if(bDebugDraw) (*itr)->drawTarget();
            (*itr)->draw();
            glPopMatrix();
            
            x+=280;
            if(x>ofGetWidth()-300){
                x = 0;
                y += 330;
            }

        }
    
        ofPopMatrix();

        glTranslatef(0, fontSize+30, 0);

    
    
        // fonst shape
    /*
        ofPushMatrix();
            ofSetColor(255, 0, 0);
            ofSetLineWidth(2);
            ofLine(0, 0, 0, -300);
        
            ofNoFill();
            ofSetColor(100, 100, 100);
        
            int posx = 0;
            for(int i=0; i<inputText.size(); i++){
                string s = ofToString(inputText[i]);
                font.drawStringAsShapes(s, posx, 0);
                
                ofRectangle r = font.getStringBoundingBox(ofToString(inputText[i]), posx, 0);
                ofNoFill();
                ofSetLineWidth(1);
                ofRect(r.x, r.y, r.width, r.height);

                posx += font.stringWidth(s);
            }
        ofPopMatrix();
*/
    
/*
        glTranslatef(0, 50, 0);
        
        // FBO check
        posx = 0;
        ofSetColor(255);
        ofPushMatrix();
        for(int i=0; i<fbos.size(); i++){
            fbos[i]->draw(posx, 0);
            posx += fbos[i]->getWidth();
        }
        ofPopMatrix();
*/
  
    
    ofPopMatrix();

    
    ofPushMatrix();
    ofTranslate(0,0);
    ofSetColor(0);
    ofFill();
    ofRect(0, 0, 200, 200);
  	ofSetColor(255);
    ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()),20,20);
//    ofDrawBitmapString("icon num: " + ofToString(pman.getInstanceNum()),20,40);
    ofDrawBitmapString("inputText.length: " + ofToString(inputText.size()),20,60);
    ofDrawBitmapString("iconSize: " + ofToString(iconSize) + " pixel",20,80);
    ofDrawBitmapString("fontSize: " + ofToString(fontSize) + " pixel",20,100);
    
    
    ofPopMatrix();
    

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
        case 'F':
            ofToggleFullscreen();
            break;
            
       case 'B':
            bDebugDraw = !bDebugDraw;
            break;
            
        case OF_KEY_RETURN:
            pictoString.clear();
            break;

        case 'C':
//        {
//            bCap = true;
//            
//        }
            break;

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
