//
//  pictoString.cpp
//  osx_picto_rev02
//
//  Created by matobahiroshi on 2013/08/27.
//
//

#include "pictoString.h"
#include "testApp.h"
#include "attractor.h"

string pictoString::text = "";

pictoString::pictoString(){
}

void pictoString::update(){
    vector<pictoChar*>::iterator itr = pchars.begin();
    vector<pictoChar*> remover;
    for(; itr!=pchars.end(); itr++){
        pictoChar * pc = (*itr);
        pc->update();
        
        if(pc->clearanceCheck()){
            remover.push_back(pc);
        }
    }
    
    vector<pictoChar*>::iterator itr2;
    for(int i=0; i<remover.size(); i++){
        clearFromPictoString(remover[i]);
    }
    
    attractor::update();
}

void pictoString::draw(){
    ofPushMatrix();{
        ofFill();
        vector<pictoChar*>::iterator itr = pchars.begin();
        for(int i=0; itr!=pchars.end(); itr++, i++){
            
            ofPushMatrix();
            ofSetRectMode(OF_RECTMODE_CENTER);
            if(testApp::getDebugDraw()){
                (*itr)->drawTarget();
            }else{
                (*itr)->draw();
            }
            ofSetRectMode(OF_RECTMODE_CORNER);

            glPopMatrix();
        }
    }ofPopMatrix();
    
    attractor::draw();
}


void pictoString::drawPreview(){
    ofBackground(255);
    
    int w = testApp::getW();
    int h = testApp::getH();
    
    ofTrueTypeFont& font = pictoChar::getFont();
    
    float scalex = (float)(305.0/(float)w);
    float scaley = (float)(183.0/(float)h);
    
    float fontScale = pictoChar::getFontScale();
    float lineHeight = font.getLineHeight();
    float letterSpacing = font.getLetterSpacing();
    
    ofPushMatrix();{
        glScalef(scalex, scaley, 1);
        
        // draw center cross
        ofLine(w/2, 0, w/2, h);
        ofLine(0, h/2, w, h/2);
        
        vector<string> lines = ofSplitString(text, "\n");
        int lineNum = 0;
        vector<float> offsetXs;
        for (int i=0; i<lines.size(); i++) {
            string thisline = lines[i];
            float sw = font.stringWidth(lines[i])*fontScale*0.5;
            offsetXs.push_back(sw);
            int posx = w/2 - offsetXs[i];
            int posy = lineHeight * i * fontScale;
            // draw bounding box for this line
            ofPushMatrix();{
                ofNoFill();
                ofSetColor(0, 0, 200);
                ofSetRectMode(OF_RECTMODE_CORNER);
                ofRect(posx, posy, offsetXs[i]*2, lineHeight*fontScale);
            }ofPopMatrix();
        }
        
        ofPushMatrix();{
            
            int posx = w/2 - offsetXs[lineNum];
            int posy = 0;
            float lineHeightScaled = lineHeight * fontScale;
            for(int i=0; i<text.size(); i++){
                char c = text.at(i);
                float charw = font.getCharProps(c).setWidth * letterSpacing * fontScale;
                
                if(c == '\n'){
                    lineNum++;
                    posx = w/2 - offsetXs[lineNum];
                    posy += lineHeightScaled;
                    continue;
                }
                
                if(posx > w - charw){
                    lineNum++;
                    posx = w/2 - offsetXs[lineNum];
                    posy += lineHeightScaled;
                }
                
                glPushMatrix();
                glTranslatef(posx, posy, 0);
                ofNoFill();
                ofSetColor(0,0,0);
                pictoChar::drawFontText(ofToString(c), 0, 0);
                glPopMatrix();
                
                posx += charw;
            }
            
        }ofPopMatrix();
    }ofPopMatrix();
}



void pictoString::clearAll(){
    if(pchars.size()>0){
        for(int h=0; h<pchars.size(); h++){
            pictoChar * pchar = pchars[h];
            pchar->clearAnimation(200+h*50);
        }
    }
}

void pictoString::clearFromPictoString(pictoChar *c){
    vector<pictoChar*>::iterator itr;
    itr = std::find(pchars.begin(), pchars.end(), c);
    if(itr != pchars.end()){
        pictoChar * pc = (*itr);
        delete pc;
        pc = 0;
        pchars.erase(itr);
        
    }
}


void pictoString::makeAnimation(){
    
    string s = text;
    
    int w = testApp::getW();
    int h = testApp::getH();
    
    ofTrueTypeFont& font = pictoChar::getFont();
    float fontScale = pictoChar::getFontScale();
    
    vector<string> lines = ofSplitString(s, "\n");
    vector<float> offsetXs;
    for (int i=0; i<lines.size(); i++) {
        float sw = font.stringWidth(lines[i]) * fontScale * 0.5;
        offsetXs.push_back(sw);
    }
    
    int rx = testApp::getW() * 0.5;
    int ry = testApp::getH() * 0.5;
    
    int lineNum = 0;
    int posx = w/2 - offsetXs[lineNum];
    int posy = 0;
    int spacex = 45;
    int spacey = 45;
    float lineHeight = font.getLineHeight() * fontScale;
    float letterSpacing = font.getLetterSpacing();
    float letterHeight = font.stringHeight("1") * fontScale;
    
    vector<ofVec2f> ps1, ps2, ps3;
    vector<ofVec2f> charPosList;
    
    for(int i=0; i<s.size(); i++){
        char c = s.at(i);
        float charw = font.getCharProps(c).setWidth * letterSpacing * fontScale;
        
        if(c == '\n'){
            lineNum++;
            posx = w/2 - offsetXs[lineNum];
            posy += lineHeight;
            continue;
        }
        
        if(posx > w - charw){
            lineNum++;
            posx = w/2 - offsetXs[lineNum];
            posy += lineHeight;
        }
        
        ofVec2f charPos = ofVec2f(posx, posy);
        charPosList.push_back(ofVec2f(charPos.x, charPos.y+lineHeight/2));
        
        pictoChar * pchar = new pictoChar(c, charPos, this);
        posx += charw;
        
        pchars.push_back(pchar);
//
//        int n = pchar->getInstanceNum();
//        ofVec2f rand1 = ofVec2f( ofRandom(w*0.1, w*0.13), ofRandom(h*0.55, h*0.60));
//        ofVec2f rand2 = ofVec2f( ofRandom(w*0.76, w*0.82), ofRandom(h*0.55, h*0.60));
//        
//        float rangeh = w*0.07;
//        float rangew = rangeh * 1.3;
//        
//        int time = 100;
//        float rate = 1;
//        pchar->setRandomAnimation(time*rate, rand1, rangew, rangeh, true);
//
//        time += 1200;
//        pchar->setRandomAnimation(time*rate, rand2, rangew, rangeh, true);
//        
//        time += 3800;
//        pchar->setRandomAnimation(time*rate, charPosList[0], rangew*1.2, rangeh*1.2, true);
//
//        time += 2000;
//        for(int j=0; j<charPosList.size(); j++){
//            pchar->setRandomAnimation(time, charPosList[j], rangew*1.3, rangeh*1.3, false);
//            time += 500;
//        }
//        
//        time += 30;
        
        int time = 7000;
        if(i>0){
            time+=4000;
        }
        time += i*600;
        attractor::addAttraction(time, ofVec2f(charPos.x, charPos.y + lineHeight-letterHeight*0.5));
        
        time += 100;
        pchar->setFinalAnimation(time, false);
        
//        pchar->printAnimationList();
    
    }
    int time = 400;

    
    ofVec2f rand1 = ofVec2f( ofRandom(w*0.3, w*0.4), ofRandom(h*0.55, h*0.70));
    ofVec2f rand2 = ofVec2f( ofRandom(w*0.55, w*0.65), ofRandom(h*0.55, h*0.70));

    attractor::addAttraction(time, rand2);

    time += 1000;
    attractor::addAttraction(time, rand1);
    
    time += 3000;
    attractor::addAttraction(time, rand2);
    
}


vector<picto*> pictoString::seek(const ofVec2f& p1, float distance){
    vector<picto*> friends;
    
    PICTO_STR_ITR itr = pchars.begin();
    for(; itr!= pchars.end(); itr++){
        pictoChar * pchar = (*itr);
        pictoChar::PCON& pcon = pchar->getPcon();
        pictoChar::PITR pitr = pcon.begin();
        for(; pitr!= pcon.end(); pitr++){
            picto * pic = (*pitr);
            ofVec2f p2 = pic->getPos();
            float d = p1.distance(p2);
            if(0<d && d<distance){
                friends.push_back(pic);
            }
        }
    }
    return friends;
}





