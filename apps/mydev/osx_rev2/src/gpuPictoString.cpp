//
//  gpuPictoString.cpp
//  osx_picto
//
//  Created by matobahiroshi on 2013/08/30.
//
//


#include "testApp.h"
#include "gpuPictoString.h"
#include "gpuPictoChar.h"
#include "gpuPicto.h"
#include "attractor.h"

float gpuPictoString::FONT_SIZE;
float gpuPictoString::ICON_SIZE;
float gpuPictoString::FONT_RANDOMNESS;
float gpuPictoString::ICON_DENSITY;
float gpuPictoString::LINE_HEIGHT;
float gpuPictoString::LETTER_SPACING;
float gpuPictoString::VIBRATION;
float gpuPictoString::SPEED;
float gpuPictoString::ACCEL;

gpuPictoString::gpuPictoString(){
    string fontName = "HelveticaNeueCondensedBold.ttf";
    font.loadFont("type/"+fontName, 500, true, true, true);

    imgSize = 32;

    // manual mipmap
    img1.loadImage("png/picto_all_star_1_black.png");
    img2.loadImage("png/picto_all_star_2_black.png");
    img4.loadImage("png/picto_all_star_4_black.png");
    img8.loadImage("png/picto_all_star_8_black.png");
    img16.loadImage("png/picto_all_star_16_black.png");
    img32.loadImage("png/picto_all_star_32_black.png");
    img64.loadImage("png/picto_all_star_64_black.png");
    img128.loadImage("png/picto_all_star_128_black.png");
    
    setup();

    bNeedUpdateCharPos = true;
}



void gpuPictoString::setup(){
    particleSize = 30.0f;
    timeStep = 0.0167f;
    numParticles = 512*512;
    
    offset.set(0.5, 0.5);
    offsetVel.set(0, 0);
    
    updatePos.load("", "shaders/posUpdate.frag");
    updateVel.load("", "shaders/velUpdate.frag");
    
    updateRender.setGeometryInputType(GL_POINTS);
    updateRender.setGeometryOutputType(GL_TRIANGLE_STRIP);
    updateRender.setGeometryOutputCount(6);
    updateRender.load("shaders/render.vert", "shaders/render.frag", "shaders/render.geom");
    
    textureRes = (int)sqrt((float)numParticles);
    numParticles = textureRes*textureRes;
    printf("Particle Num Limited under %i\n", numParticles);
    
    
    
    finalTargetTex.allocate(textureRes, textureRes, GL_RGB32F);
    iconPrmTex.allocate(textureRes, textureRes, GL_RGBA32F);
    springPrmTex.allocate(textureRes, textureRes, GL_RGBA32F);
    randomTex.allocate(textureRes, textureRes, GL_RGB32F);
    
    
    // target
    finalTargetPosData = new float[numParticles*3];
    
    // random
    randomData = new float[numParticles*3];
    for (int x = 0; x < textureRes; x++){
        for (int y = 0; y < textureRes; y++){
            int i = textureRes * y + x;
            
            randomData[i*3 + 0] = ofRandom(1.0);
            randomData[i*3 + 1] = ofRandom(1.0);
            randomData[i*3 + 2] = ofRandom(1.0);
//            randomData[i*4 + 3] = ofRandom(1.0);
        }
    }
    randomTex.loadData(randomData, textureRes, textureRes, GL_RGB);
    
    
    
    // icon
    iconPrmData = new float[numParticles*4];
    for (int x = 0; x < textureRes; x++){
        for (int y = 0; y < textureRes; y++){
            int i = textureRes * y + x;
            
            iconPrmData[i*4 + 0] = -0.5;  // colorType
            iconPrmData[i*4 + 1] = -0.5;  // iconType
            iconPrmData[i*4 + 2] = 0.0;
            iconPrmData[i*4 + 3] = 0.0;
        }
    }
    iconPrmTex.loadData(iconPrmData, textureRes, textureRes, GL_RGBA);

    
    
    //spring
    springPrmData = new float[numParticles*4];
    for (int x = 0; x < textureRes; x++){
        for (int y = 0; y < textureRes; y++){
            int i = textureRes * y + x;
            
            springPrmData[i*4 + 0] = ofRandom(0.08, 0.17); // K
            springPrmData[i*4 + 1] = ofRandom(0.3, 0.55); // topSpeed
            springPrmData[i*4 + 2] = ofRandom(0.001, 0.03); // minSpeed;
            springPrmData[i*4 + 3] = 1.0;           // attractOn
            
            if(ofRandom(0, 100)>99){
                springPrmData[i*4 + 2] = ofRandom(0.15, 0.2);
            }
        }
    }
    springPrmTex.loadData(springPrmData, textureRes, textureRes, GL_RGBA);

    
    
    // pos
    posData = new float[numParticles*3];
    for (int x = 0; x < textureRes; x++){
        for (int y = 0; y < textureRes; y++){
            int i = textureRes * y + x;
            
            posData[i*3 + 0] = 0.0; //x;
            posData[i*3 + 1] = 0.0; //y;
            posData[i*3 + 2] = 0.0;
        }
    }
    
    
    posPingPong.allocate(textureRes, textureRes, GL_RGB32F);
    posPingPong.src->getTextureReference().loadData(posData, textureRes, textureRes, GL_RGB);
    posPingPong.dst->getTextureReference().loadData(posData, textureRes, textureRes, GL_RGB);
    
    
    // vel
    velData = new float[numParticles*4];
    for (int i = 0; i < numParticles; i++){
        velData[i*4 + 0] = 0;
        velData[i*4 + 1] = 0;
        velData[i*4 + 2] = 0; //a
        velData[i*4 + 3] = 0; //a
        
    }
    velPingPong.allocate(textureRes, textureRes,GL_RGBA32F);
    velPingPong.src->getTextureReference().loadData(velData, textureRes, textureRes, GL_RGBA);
    velPingPong.dst->getTextureReference().loadData(velData, textureRes, textureRes, GL_RGBA);
    
    
    int w = ofGetWindowWidth();
    int h = ofGetWindowHeight();
    renderFBO.allocate(w, h, GL_RGB32F);
    renderFBO.begin();
    ofClear(0,0,0,255);
    renderFBO.end();
}


vector<ofVec3f> gpuPictoString::calcCharPos(){
    string s = text;
    
    int w = testApp::getW();
    int h = testApp::getH();
    
    float fontScale = getFontScale();

    // font のLineHeightに影響されない
    float height1 = font.getCharProps('1').height;
    
    font.setLetterSpacing(LETTER_SPACING);
    font.setLineHeight(LINE_HEIGHT*height1);
    
    vector<string> lines = ofSplitString(s, "\n");
    vector<float> offsetXs;
    for (int i=0; i<lines.size(); i++) {
        float sw = font.stringWidth(lines[i]) * fontScale * 0.5;
        offsetXs.push_back(sw);
    }
    
    int lineNum = 0;
    int posx = w/2 - offsetXs[lineNum];
    int posy = 0;
    float lineHeight = font.getLineHeight() * fontScale;
    float letterSpacing = font.getLetterSpacing();
    float letterHeight = height1 * fontScale;
    
    vector<ofVec2f> ps1, ps2, ps3;
    vector<ofVec3f> charPosList;
    
    for(int i=0; i<s.size(); i++){
        char c = s.at(i);
        float charw = font.getCharProps(c).setWidth * letterSpacing * fontScale;
        
//        if(posx > w - charw){
//            lineNum++;
//            posx = w/2 - offsetXs[lineNum];
//            posy += lineHeight;
//        }

        ofVec3f charPos(posx, posy, (int)c);
        charPosList.push_back(charPos);
        
        if(c == '\n'){
            lineNum++;
            posx = w/2 - offsetXs[lineNum];
            posy += lineHeight;
            continue;
        }else{
            posx += charw;
        }
    }
    
    return charPosList;
}

string gpuPictoString::alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-^@[;:],./_!\"#$%&'()=~|`{+*}<>?_\\\n";

void gpuPictoString::makeAnimation(){
    
    clearAll();
    finalTargets.clear();
    gpuPicto::totalPicto = 0;

    offset.set(ofRandom(0.0,1.0),ofRandom(0.1,0.9));
    pastOffset = offset;
    offsetVel.set(ofRandom(-0.5,0.5),ofRandom(-0.5,0.5));
    
    float w = testApp::getW();
    float h = testApp::getH();
    
    int time = 0;
    {
        // attractor
        ofVec2f randL = ofVec2f( ofRandom(0.1, 0.2), ofRandom(0.55, 0.70));
        ofVec2f randR = ofVec2f( ofRandom(0.8, 0.9), ofRandom(0.55, 0.70));

        attractor::addAttraction(time, randL);
        
        time += 2000;
        attractor::addAttraction(time, randR);
        
        //time += 2000;
        //attractor::addAttraction(time, randR);

    }

//    if(bNeedUpdateCharPos){
        charPosList = calcCharPos();
        bNeedUpdateCharPos = false;
//    }
    
    float fontScale = getFontScale();
    float lineHeight = font.getLineHeight() * fontScale;
    float letterHeight = font.stringHeight("1") * fontScale;
    float res = lineHeight * ICON_DENSITY;
    float rand = lineHeight * FONT_RANDOMNESS;

    int index = 0;
    
//    cout << endl << "Start Make Pictograph " << endl;
    
    char c;
    char pastc;
    for(int i=0; i<charPosList.size(); i++){
        if(gpuPicto::totalPicto>=numParticles) break;
        ofVec3f xyc = charPosList[i];
        c = (char)xyc.z;
        int find = alphabet.find(c);
        if(find == -1 || c == '\302' || c == '\245'){
//            cout << "skip " << c << endl;
            continue;
        }else{
//            cout << "make " << c << endl;
        }
        
        if(c != ' ' && c!='\n'){
            gpuPictoChar * gpchar = new gpuPictoChar(c, xyc.x, xyc.y, this);
            gpchars.push_back(gpchar);

            // target
            gpchar->getFinalTarget(font, getFontScale(), res, rand, finalTargets);
            
            int n = gpchar->numPicto;
            
            for (int j=0; j<n; j++) {
                if(gpuPicto::totalPicto>=numParticles) break;
                gpuPicto::totalPicto++;
                
                // target
                finalTargetPosData[index*3    ] = (xyc.x + finalTargets[index*3  ]) / w;
                finalTargetPosData[index*3 + 1] = (xyc.y + finalTargets[index*3 + 1]) / h;
                finalTargetPosData[index*3 + 2] = 0;

                // pos
                posData[index*3    ] = pastOffset.x + ofRandom(-0.1, 0.1); //ofRandom(0.45, 0.55);
                posData[index*3 + 1] = pastOffset.y + ofRandom(-0.1, 0.1); //ofRandom(0.45, 0.55);
                posData[index*3 + 2] = ofRandom(0.00001, 0.01); // alpha
                
                // vel
                velData[index*4    ] = ofRandom(-0.5, 0.5);
                velData[index*4 + 1] = ofRandom(-0.5, 0.5);
                velData[index*4 + 2] = 0;
                velData[index*4 + 3] = 0;
                
                
                // icon
                iconPrmData[index*4 + 0] = ofRandom(1.0);   // colorType
                iconPrmData[index*4 + 1] = ofRandom(1.0);   // iconType
                iconPrmData[index*4 + 2] = 0.0;
                iconPrmData[index*4 + 3] = 0.0;
                
                //spring
                springPrmData[index*4 + 3] = 1;         // attractOn
                index++;
            }
        

            
            // attractor
            if(i<=1){
                time += 1800;
            }else{
                if(pastc=='\n'){
                    time += 20;
                }else{
                    time += 250;
                }
            }
            
            attractor::addAttraction(time, ofVec2f((xyc.x/w - 0.5)*1.2 + 0.5, (xyc.y+lineHeight-letterHeight*0.5)/h) );
            
            int nowf = ofGetFrameNum();
            gpchar->spreadFrame = nowf + (float)(time+200)/1000.0*60.0;
        }
        if(c =='\n'){
            time += 500;
        }else if(pastc=='\n'){
            time += 1000;
        }
        pastc = c;
    }

    int total = gpuPicto::totalPicto;
    
    int pixTotal = textureRes * textureRes;

    finalTargetTex.loadData(finalTargetPosData, textureRes, textureRes, GL_RGB);
    iconPrmTex.loadData(iconPrmData, textureRes, textureRes, GL_RGBA);
    springPrmTex.loadData(springPrmData, textureRes, textureRes, GL_RGBA);

    posPingPong.src->getTextureReference().loadData(posData, textureRes, textureRes, GL_RGB);
    posPingPong.dst->getTextureReference().loadData(posData, textureRes, textureRes, GL_RGB);
    
    velPingPong.src->getTextureReference().loadData(velData, textureRes, textureRes, GL_RGBA);
    velPingPong.dst->getTextureReference().loadData(velData, textureRes, textureRes, GL_RGBA);
  
    
//    for(int i=0; i<total*3; i++){
//        cout << i << " : " << finalTargetPosData[i] << endl;
//    }
    
}

float fx = 0;
float fy = 0;
void gpuPictoString::update(){

    float w = testApp::getW();
    float h = testApp::getH();
    
    if(bNeedUpdateCharPos){
        charPosList = calcCharPos();
        bNeedUpdateCharPos = false;
    }
    
    int total = gpuPicto::totalPicto;
    int totalPix = textureRes*textureRes;
    
    // manual mipmap
    //                              *** should check icon size
    ofImage * img = NULL;
    
    int iconSize = ICON_SIZE * h;
    
    if(iconSize<=1){
        img = &img1;
        imgSize = 1;
    }else if(iconSize<=2){
        img = &img2;
         imgSize = 2;
    }else if (iconSize<=4){
        img = &img4;
         imgSize = 4;
    }else if (iconSize<=8){
        img = &img8;
        imgSize = 8;
    }else if (iconSize<=16){
        img = &img16;
        imgSize = 16;
    }else if (iconSize<=32){
        img = &img32;
        imgSize = 32;
    }else if(iconSize<=64){
        img = &img64;
        imgSize = 64;
    }else{
        img = &img128;
        imgSize = 128;
    }

    iconSize*=0.5;
    if(iconSize<1){ iconSize=1; }
    
    //
    //  nomad
    //
    if(total>0 && ofRandom(1.0)>0.92){
        
        if(gpchars.size()>2){
            int charIndexA = (int)ofRandom(2, gpchars.size()-2);
            int charIndexB = charIndexA + (int)ofRandom(-2, 2);
        
            if(charIndexA!=charIndexB){
                int numPictoA = gpchars[charIndexA]->numPicto;
                int numPictoB = gpchars[charIndexB]->numPicto;
                int firstIdA = gpchars[charIndexA]->firstIndex;
                int firstIdB = gpchars[charIndexB]->firstIndex;
                
                int indexA = ofRandom(firstIdA, numPictoA-1);
                int indexB = ofRandom(firstIdB, numPictoB-1);
                
                float attractOnA = springPrmData[indexA*4+3];
                float attractOnB = springPrmData[indexB*4+3];
                
                if(attractOnA<0 && attractOnB<0){
                    finalTargetPosData[indexA*3 + 0] = finalTargetPosData[indexB*3 + 0];
                    finalTargetPosData[indexA*3 + 1] = finalTargetPosData[indexB*3 + 1];

                    finalTargetTex.loadData(finalTargetPosData, textureRes, textureRes, GL_RGB);
                }
            }
        }
    }
    
    
    {
        const ofVec2f& attr = attractor::getPos();

        float K = 20;
        float maxSpeed = 0.05;

        ofVec2f dir = attr - offset;
        ofVec2f acc =  K * 0.0000167 * dir;
        offsetVel += acc;
        
        offsetVel.limit(maxSpeed);
        offset += offsetVel;
        
        ofVec2f dirf = attr - offset;
        if(dirf.length() > 0.1){
            offset += dirf - dirf.normalized() * 0.1;
        }
    }
    
    //
    //  check pictoChar
    //
    bool shouldUpdateSpringTexture = false;
    GPICTO_STR_ITR itr = gpchars.begin();
    int particleMax = textureRes*textureRes;
    for(; itr!=gpchars.end(); itr++){
        
        if((*itr)->update()){
            shouldUpdateSpringTexture = true;
            int index = (*itr)->firstIndex;
            int numPicto = (*itr)->numPicto;

            if(index<=particleMax){

                for(int i=0; i<numPicto; i++){
                    springPrmData[index*4 + 3] = -1;    // attractOn
                    
                    index++;
                    if(index>particleMax){
                        index = 0;
                    }
                }
            }
        }
    }
    
    
    if(shouldUpdateSpringTexture){
        springPrmTex.loadData(springPrmData, textureRes, textureRes, GL_RGBA);
    }
    
    
    //
    // make random tex
    //
//    if(ofGetFrameNum()%3 == 0){
        for(int index=0; index<total; index++){
                randomData[index*3 + 0] = ofRandom(1.0);
                randomData[index*3 + 1] = ofRandom(1.0);
                randomData[index*3 + 2] = ofRandom(1.0);
//                randomData[index*4 + 3] = ofRandom(1.0);
        }
        randomTex.loadData(randomData, textureRes, textureRes, GL_RGB);
//    }


    //
    // 1. calc vel
    //
    velPingPong.dst->begin();{
        ofClear(0);
        updateVel.begin();{
            updateVel.setUniformTexture("backbuffer", velPingPong.src->getTextureReference(), 0);
            updateVel.setUniformTexture("posData", posPingPong.src->getTextureReference(), 1);
            updateVel.setUniformTexture("springData", springPrmTex, 2);
            updateVel.setUniformTexture("randomData", randomTex, 3);
            updateVel.setUniformTexture("targetData", finalTargetTex, 4);
            
            updateVel.setUniform1i("resolution", (int)textureRes);
            updateVel.setUniform2f("screen", (float)width, (float)height);
            updateVel.setUniform2f("offset", (float)offset.x, (float)offset.y);
            updateVel.setUniform2f("pastOffset", (float)pastOffset.x, (float)pastOffset.y);

            updateVel.setUniform1f("timestep", (float)timeStep);
            updateVel.setUniform1f("ACCEL",(float) ACCEL );
            updateVel.setUniform1f("SPEED",(float) SPEED );
            updateVel.setUniform1f("VIBRATION",(float) VIBRATION );
            
            // draw the source velocity texture to be updated
            velPingPong.src->draw(0, 0);
        }updateVel.end();
    }velPingPong.dst->end();
    
    velPingPong.swap();
    
    
    //
    // 2. calc pos
    //
    posPingPong.dst->begin();{
        ofClear(0);
        updatePos.begin();{
            updatePos.setUniformTexture("prevPosData", posPingPong.src->getTextureReference(), 0); // Previus position
            updatePos.setUniformTexture("velData", velPingPong.src->getTextureReference(), 1);  // Velocity
            updatePos.setUniformTexture("springData", springPrmTex, 2);

            updatePos.setUniform1f("timestep",(float) timeStep );
            updatePos.setUniform2f("offset", (float)offset.x, (float)offset.y);
            updatePos.setUniform2f("pastOffset", (float)pastOffset.x, (float)pastOffset.y);
            posPingPong.src->draw(0, 0);
        }updatePos.end();
    }posPingPong.dst->end();
    
    posPingPong.swap();
    
    
    
    //
    // 3. render
    //
    renderFBO.begin();
    ofClear(testApp::getBackgroundColor()); // ofClear(0);
    updateRender.begin();
    updateRender.setUniformTexture("posTex", posPingPong.dst->getTextureReference(), 0);
    updateRender.setUniformTexture("sparkTex", img->getTextureReference() , 1);
    updateRender.setUniformTexture("iconPrmTex", iconPrmTex, 2);
    updateRender.setUniformTexture("springData", springPrmTex, 3);

    updateRender.setUniform1i("resolution", (float)textureRes);
    updateRender.setUniform2f("screen", (float)testApp::getW(), (float)testApp::getH());
    updateRender.setUniform1i("size", (int)iconSize);
    updateRender.setUniform1f("imgWidth", imgSize);
    updateRender.setUniform1f("imgHeight", imgSize);
    updateRender.setUniform2f("offset", (float)offset.x, (float)offset.y);

    ofPushStyle();
    //ofEnableBlendMode( OF_BLENDMODE_ADD );
    ofEnableBlendMode( OF_BLENDMODE_ALPHA);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    ofSetColor(255);
    
    glBegin( GL_POINTS );
    int count = 0;
    
    for(int y = 0; y < textureRes; y++){
        for(int x = 0; x < textureRes; x++){
          if(count >= total) break;
            count++;

            glVertex2d(x,y);
            glTexCoord2i(x, y);
        }
    }
    
    ofDisableBlendMode();
    glEnd();
    
    updateRender.end();
    renderFBO.end();
    ofPopStyle();
    
    pastOffset = offset;
}


void gpuPictoString::draw(){
    
    int w = testApp::getW();
    int h = testApp::getH();

    ofSetColor(255,255,255);
    renderFBO.draw(0,0);

    if(testApp::getDebugDraw()){
        {
            // Attractor
            const ofVec2f& attr = attractor::getPos();
            ofFill();
            ofSetColor(255, 55, 0);
            ofRect(attr.x*w, attr.y*h, 10, 10);
        }
        
        if(false){
            // FBO check
            float scale = getFontScale();
            float lineHeightScaled = font.getLineHeight()*scale;
            
            GPICTO_STR_ITR itr = gpchars.begin();
            int particleMax = textureRes*textureRes;
            ofSetColor(255);
            for(; itr!=gpchars.end(); itr++){
                ofVec2f& pos = (*itr)->charPos;
                glPushMatrix();
                glTranslatef(pos.x, pos.y, 0);
                (*itr)->drawFbo();
                glPopMatrix();
            }
        }
        
        {
            // + line
            ofSetColor(0, 0, 255);
            glBegin(GL_LINES);
            glVertex3f(w/2, 0, 0); glVertex3f(w/2, h, 0);
            glVertex3f(0, h/2, 0); glVertex3f(w, h/2, 0);
            glEnd();
        }
    }
}



void gpuPictoString::drawPreview(){

    ofBackground(255);
    int w = testApp::getW();
    int h = testApp::getH();
    
float screenScalex = 305.0/(float)w;
    float screenScaley = 183.0/(float)h;
    
    float scale = getFontScale();
    float fh = font.getLineHeight();    // ????

    glPushMatrix();{
        glScalef(screenScalex, screenScaley, 1);
        for(int i=0; i<charPosList.size(); i++){
            ofVec3f& xyc = charPosList[i];
            char c = (char)xyc.z;
            int find = alphabet.find(c);
            if(find == -1 || c == '\302' || c == '\245'){
                //            cout << "skip " << c << endl;
                continue;
            }else{
                //            cout << "make " << c << endl;
            }
            
            glPushMatrix();{
                glTranslatef(xyc.x, xyc.y, 0);
                ofNoFill();
                ofSetColor(120);
            
                glScalef(scale, scale, 1);
                
                /* 
                    Here, Font y position (fh * 1.2)
                    Sohuld use same value with gpuPictoChar::getFinalTarget
                 
                    DO NOT USE font.drawString()
                    This will make app crash. I think due to Cocoa multiple GL view (and shaders).
                */
                
                font.drawStringAsShapes(ofToString(c), 0, fh*1.2);
                
                // ofRect(10, 10, 100, 100);
            }glPopMatrix();
        }
        
        ofSetColor(0, 0, 255);
        glBegin(GL_LINES);
        glVertex3f(w/2, 0, 0); glVertex3f(w/2, h, 0);
        glVertex3f(0, h/2, 0); glVertex3f(w, h/2, 0);
        glEnd();
        
    }glPopMatrix();
    
    
}

void gpuPictoString::clearAll(){
    // target
    int w = testApp::getW();
    int h = testApp::getH();
    int total = gpuPicto::totalPicto;
    
    for(int index=0; index<total; index++){
        finalTargetPosData[index*3    ] += 1.5;
        finalTargetPosData[index*3 + 1] += ofRandom(-0.2, 0.2);
        
        springPrmData[index*4+3] = -1;
    }
    
    finalTargetTex.loadData(finalTargetPosData, textureRes, textureRes, GL_RGB);
    springPrmTex.loadData(springPrmData, textureRes, textureRes, GL_RGBA);
    
    attractor::getAttractions().clear();
    attractor::resetPos();
    
    GPICTO_STR_ITR itr = gpchars.begin();
    for(; itr!=gpchars.end(); itr++){
        gpuPictoChar * gc = (*itr);
        delete gc;
    }
    gpchars.clear();

//    offset.set(0.5, 0.5);
//    offsetVel.set(0,0);
//    pastOffset.set(0.5, 0.5);
    
    //gpuPicto::totalPicto = 0;
    
}

void gpuPictoString::setRenderFboResolution(float w, float h){
    // windowing
    if(renderFBO.isAllocated()){
        renderFBO.allocate(w, h, GL_RGB32F);
        renderFBO.begin();
        ofClear(0,0,0,255);
        renderFBO.end();
    }
}

