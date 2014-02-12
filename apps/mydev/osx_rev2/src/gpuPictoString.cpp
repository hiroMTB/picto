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

//
//  parameter set
//
PrmData::PrmData(string _message, float _fontSize, float _lineHeight, float _letterSpacing, float _fontRandomness, float _iconSize, float _iconDensity, float _speed, float _accel, float _vibration, int _holdTime)
:message(_message), fontSize(_fontSize), lineHeight(_lineHeight), letterSpacing(_letterSpacing), fontRandomness(_fontRandomness), iconSize(_iconSize), iconDensity(_iconDensity), speed(_speed), accel(_accel), vibration(_vibration), holdTime(_holdTime)
{}


PrmData::PrmData(const PrmData& rhs)
:message(rhs.message), fontSize(rhs.fontSize), lineHeight(rhs.lineHeight), letterSpacing(rhs.letterSpacing), fontRandomness(rhs.fontRandomness), iconSize(rhs.iconSize), iconDensity(rhs.iconDensity), speed(rhs.speed), accel(rhs.accel), vibration(rhs.vibration), holdTime(rhs.holdTime)
{}

const PrmData& PrmData::operator=(const PrmData& rhs){
    message = rhs.message;
    fontSize = rhs.fontSize;
    lineHeight = rhs.lineHeight;
    letterSpacing = rhs.letterSpacing;
    fontRandomness = rhs.fontRandomness;
    iconSize = rhs.iconSize;
    iconDensity = rhs.iconDensity;
    speed = rhs.speed;
    accel = rhs.accel;
    vibration =rhs.vibration;
    holdTime = rhs.holdTime;
    return *this;
}

PrmData gpuPictoString::prm;

const ofColor gpuPictoString::colors[5] = {
    ofColor(   0,  194,  229),     // water blue
    ofColor( 255,  205,   10),    // yellow
    ofColor( 165,   92,  192),    // purple
    ofColor(  10,  195,   92),     // light green
    ofColor( 250,   70,   83)      // light red
};

gpuPictoString::gpuPictoString(){
    string fontName = "HelveticaNeueCondensedBold.ttf";
    font.loadFont("type/"+fontName, 500, true, true, true);

    imgSize = 32;

    // manual mipmap
    img1.loadImage("png/rev02/picto_all_star_1_black_rev02.png");
    img2.loadImage("png/rev02/picto_all_star_2_black_rev02.png");
    img4.loadImage("png/rev02/picto_all_star_4_black_rev02.png");
    img8.loadImage("png/rev02/picto_all_star_8_black_rev02.png");
    img16.loadImage("png/rev02/picto_all_star_16_black_rev02.png");
    img32.loadImage("png/rev02/picto_all_star_32_black_rev02.png");
    img64.loadImage("png/rev02/picto_all_star_64_black_rev02.png");
    img128.loadImage("png/rev02/picto_all_star_128_black_rev02.png");
    
    setup();

    bNeedUpdateCharPos = true;

    //
    //
    //
    for(int i=0; i<numIcon; i++){
        svg[i].load("svg/picto_" + ofToString(i)+".svg");
        int n = svg[i].getNumPath();
        for(int j=0; j<n; j++){
            svg[i].getPathAt(j).setUseShapeColor(false);
        }
    }
    
    img = NULL;
    
    bClear = false;
    clearFrame = -1;
    
    bShouldStartNext = false;
    shouldStartNextFrame = -1;
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
    randomTex.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
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
    iconPrmTex.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    iconPrmTex.loadData(iconPrmData, textureRes, textureRes, GL_RGBA);

    
    
    //spring
    springPrmData = new float[numParticles*4];
    for (int x = 0; x < textureRes; x++){
        for (int y = 0; y < textureRes; y++){
            int i = textureRes * y + x;
            
            springPrmData[i*4 + 0] = ofRandom(0.06, 0.12); // K
            springPrmData[i*4 + 1] = ofRandom(0.2, 0.35); // topSpeed
            springPrmData[i*4 + 2] = ofRandom(0.001, 0.03); // minSpeed;
            springPrmData[i*4 + 3] = 1.0;           // attractOn
            
            if(ofRandom(0, 100)>99){
                springPrmData[i*4 + 2] = ofRandom(0.15, 0.2);
            }
        }
    }
    springPrmTex.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
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
    renderFBO.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    // renderFBO.getTextureReference().setTextureMinMagFilter(GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST);

    renderFBO.begin();
    ofClear(0,0,0,0);
    renderFBO.end();
    

    //img128.getTextureReference().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);                                // flickering
    //img128.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);                              // sliced
    //img128.getTextureReference().setTextureMinMagFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR);    // not so good
    //img128.getTextureReference().setTextureMinMagFilter(GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST);
    //img128.getTextureReference().setTextureMinMagFilter(GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST);
    //img128.getTextureReference().setTextureMinMagFilter(GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_LINEAR);
    
    
    img1.getTextureReference().setTextureMinMagFilter(GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_LINEAR);
    img2.getTextureReference().setTextureMinMagFilter(GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_LINEAR);
    img4.getTextureReference().setTextureMinMagFilter(GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_LINEAR);
    img8.getTextureReference().setTextureMinMagFilter(GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_LINEAR);
    img16.getTextureReference().setTextureMinMagFilter(GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_LINEAR);
    img32.getTextureReference().setTextureMinMagFilter(GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_LINEAR);
    img64.getTextureReference().setTextureMinMagFilter(GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_LINEAR);
    img128.getTextureReference().setTextureMinMagFilter(GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_LINEAR);
    
//    texadv.allocate(testApp::getW(), testApp::getH(), GL_RGB);
    //texId = renderFBO.getTextureReference().getTextureData().textureID;
    resize(testApp::getW(), testApp::getH());
}


vector<ofVec3f> gpuPictoString::calcCharPos(){
    string s = prm.message;
    
    int w = testApp::getW();
    int h = testApp::getH();
    
    float fontScale = getFontScale();

    // font のLineHeightに影響されない
    float height1 = font.getCharProps('1').height;
    
    font.setLetterSpacing(prm.letterSpacing);
    font.setLineHeight(prm.lineHeight*height1);
    
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
    
    printf("makeAnimation with prm = message:%s, fontSize:%0.4f, lineHight:%0.4f, letterSpacing:%0.4f, fontRandomness:%0.4f, iconSize:%0.4f, iconDensity:%0.4f, accel:%0.4f, vibration:%0.4f, holdTime:%i\n",
           prm.message.c_str(), prm.fontSize, prm.lineHeight, prm.letterSpacing, prm.fontRandomness, prm.iconSize, prm.iconDensity, prm.accel, prm.vibration, prm.holdTime);
    
    bClear = false;

    bNeedUpdateCharPos = true;
    
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
        ofVec2f randL = ofVec2f( ofRandom(0.2, 0.3), ofRandom(0.55, 0.70));
        ofVec2f randR = ofVec2f( ofRandom(0.7, 0.8), ofRandom(0.55, 0.70));

        //attractor::addAttraction(time, randL);
        
        time += 1500;
        attractor::addAttraction(time, randR);
        
//        time += 1500;
//        attractor::addAttraction(time, randL);

    }

//    if(bNeedUpdateCharPos){
        charPosList = calcCharPos();
        bNeedUpdateCharPos = false;
//    }
    
    float fontScale = getFontScale();
    float lineHeight = font.getLineHeight() * fontScale;
    float letterHeight = font.stringHeight("1") * fontScale;
    float res = lineHeight * (0.21-prm.iconDensity);
    float rand = lineHeight * prm.fontRandomness;
    
    if(res<0)res = 0.2;

    int index = 0;
    
//    cout << endl << "Start Make Pictograph " << endl;
    
    char c;
    char pastc;
    
    ofVec2f posMainPoint(ofRandom(0.0, 1.0), ofRandom(0.3, 0.9));
    ofVec2f velMainDir(ofRandom(-0.2, 0.2), ofRandom(-0.3, 0.3));
    
    int finalSpreadFrame = 0;
    
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
                float radian = ofRandom(0.0, 360.0)/360.0 * TWO_PI;
                float length = ofRandom(0.001, 0.1);
                if(length<0.5){length+=ofRandom(0.001, 0.04);}
                posData[index*3    ] = cos(radian) * length*1.33 + posMainPoint.x;
                posData[index*3 + 1] = sin(radian) * length + posMainPoint.y;
                posData[index*3 + 2] = ofRandom(0.1, 0.2); // alpha
                
                // vel
                velData[index*4    ] = velMainDir.x + ofRandom(-0.02, 0.02);
                velData[index*4 + 1] = velMainDir.y + ofRandom(-0.02, 0.02);
                velData[index*4 + 2] = 0;
                velData[index*4 + 3] = 0;
                
                // icon
                iconPrmData[index*4 + 0] = ofRandom(0,5) * 0.01;
                iconPrmData[index*4 + 1] = ofRandom(0,numIcon) * 0.01;    // iconType 0-0.43
                
                iconPrmData[index*4 + 2] = 0.0;
                iconPrmData[index*4 + 3] = 0.0;
                
                //spring
                springPrmData[index*4 + 3] = 1;         // attractOn
                index++;
            }
        

            
            // attractor
            //if(i<=1){
            //    time += 3000;
            //}else{
            //    time += 300;
            //}
            
            //attractor::addAttraction(time, ofVec2f((xyc.x/w - 0.5)*1.2 + 0.5, (xyc.y+lineHeight-letterHeight*0.5)/h) );
            
            int nowf = ofGetFrameNum();
            gpchar->spreadFrame = nowf + (float)(time+200)/1000.0*60.0;
            cout << "set spread frame: " << gpchar->spreadFrame << endl;
            finalSpreadFrame = gpchar->spreadFrame;
        }
//        if(c =='\n'){
//            time += 800;
//        }else if(pastc=='\n'){
//            time += 1500;
//        }
        pastc = c;
    }
    
    int readable_ending_estimated_frame = 500;
    
    int holdFrame = (float)prm.holdTime/1000*60.0;
    clearFrame = finalSpreadFrame + holdFrame + readable_ending_estimated_frame;

    shouldStartNextFrame = clearFrame + 500;
    
    cout    << "------------------" << endl
            << "set Clear frame: " << clearFrame << endl
            << "set next frame: " << shouldStartNextFrame << endl;
    
    // shuffle
//    {
//        cout << "index = " << index << endl;
//        for(int i=0; i<index; i++){
//            int rind = ofRandom(0,index);
//            float f1 = iconPrmData[i*4 +1];
//            float f2 = iconPrmData[rind*4 +1];
//            cout << "f1= " << f1 << ", f2= " << f2 << endl;
//            
//            iconPrmData[i*4 + 1]    = f2;
//            iconPrmData[rind*4 +1]  = f1;
//        }
//        
//        for(int i=0; i<index; i++){
//            cout << iconPrmData[i*4 + 1] << endl;
//        }
//    }


    int total = gpuPicto::totalPicto;
    
    int pixTotal = textureRes * textureRes;

    finalTargetTex.loadData(finalTargetPosData, textureRes, textureRes, GL_RGB);
    iconPrmTex.loadData(iconPrmData, textureRes, textureRes, GL_RGBA);
    springPrmTex.loadData(springPrmData, textureRes, textureRes, GL_RGBA);

    posPingPong.src->getTextureReference().loadData(posData, textureRes, textureRes, GL_RGB);
    posPingPong.dst->getTextureReference().loadData(posData, textureRes, textureRes, GL_RGB);
    
    velPingPong.src->getTextureReference().loadData(velData, textureRes, textureRes, GL_RGBA);
    velPingPong.dst->getTextureReference().loadData(velData, textureRes, textureRes, GL_RGBA);
    
}


void gpuPictoString::resizeIcon(int h){
    int iconSize = prm.iconSize * h;

    if(iconSize<=1){
        img = &img1;
        imgSize = 1;
    }else if(iconSize<=2){
        img = &img2;
         imgSize = 2;
    }else if (iconSize<=6){
        img = &img4;
         imgSize = 4;
    }else if (iconSize<=12){
        img = &img8;
        imgSize = 8;
    }else if (iconSize<=20){
        img = &img16;
        imgSize = 16;
    }else if (iconSize<=45){
        img = &img32;
        imgSize = 32;
    }else if(iconSize<=100){
        img = &img64;
        imgSize = 64;
    }else{
        img = &img128;
        imgSize = 128;
    }

//    printf("iconSizeChanged, iconSize=%i, prm.iconSize=%f, imgSize=%f\n", iconSize, prm.iconSize, imgSize);

}
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
    
    resizeIcon(h);
    int iconSize = prm.iconSize * h;
    iconSize*=0.5;
    if(iconSize<1){ iconSize=1; }
    

    
    //
    //  nomad
    //
    bool bNomad = true;
    if(bNomad){
        if(total>0 && ofRandom(1.0)>0.95){
            
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
                        float Ax = finalTargetPosData[indexA*3 + 0];
                        float Ay = finalTargetPosData[indexA*3 + 1];
                        
                        finalTargetPosData[indexA*3 + 0] = finalTargetPosData[indexB*3 + 0];
                        finalTargetPosData[indexA*3 + 1] = finalTargetPosData[indexB*3 + 1];

                        finalTargetPosData[indexB*3 + 0] = Ax;
                        finalTargetPosData[indexB*3 + 1] = Ay;

                        finalTargetTex.loadData(finalTargetPosData, textureRes, textureRes, GL_RGB);
                    }
                }
            }
        }
    }
    
    
    {
//        offset = attractor::getPos();
        
        const ofVec2f& attr = attractor::getPos();
        float K = 20;
        float maxSpeed = 0.05;

        ofVec2f dir = attr - offset;
        ofVec2f acc =  K * 0.000167 * dir;
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
        
        if((*itr)->spreadCheck()){
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
    
    
    if(clearCheck()){
        clearAll();
        bNeedUpdateCharPos = true;
    }
    
    if(testApp::bAutoPlay)
        shouldStartNextCheck();

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
            updateVel.setUniform1f("ACCEL",(float) prm.accel );
            updateVel.setUniform1f("SPEED",(float) prm.speed);
            updateVel.setUniform1f("VIBRATION",(float) prm.vibration );
            
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
//            updatePos.setUniform2f("offset", (float)offset.x, (float)offset.y);
//            updatePos.setUniform2f("pastOffset", (float)pastOffset.x, (float)pastOffset.y);
            posPingPong.src->draw(0, 0);
        }updatePos.end();
    }posPingPong.dst->end();
    
    posPingPong.swap();
    
    
    
    //
    // 3. render
    //
    renderFBO.begin();
    ofClear(testApp::getBackgroundColor()); // ofClear(0);
    //ofClear(0, 0, 0, 0);
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
//    updateRender.setUniform2f("offset", (float)offset.x, (float)offset.y);

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
        if(count >= total) break;
    }
    
    ofDisableBlendMode();
    glEnd();

    updateRender.end();
    
    
    
    
    if(testApp::getDebugDraw()){
        {
            // Attractor
            const ofVec2f& attr = attractor::getPos();
            ofFill();
            ofSetColor(255, 55, 0);
            ofRect(attr.x*w, attr.y*h, 10, 10);
            
            ofFill();
            ofSetColor(5, 255, 0);
            ofRect(pastOffset.x*w, pastOffset.y*h, 10, 10);
            
            ofFill();
            ofSetColor(0, 5, 220);
            ofRect(offset.x*w, offset.y*h, 10, 10);
        }
        
        {
            // + line
            if(testApp::bWallMapMouseAdjust){
                ofSetColor(0, 255, 0);
            }else{
                ofSetColor(255, 0, 0);
            }
            glBegin(GL_LINES);
            glVertex3f(w/2, 0, 0); glVertex3f(w/2, h, 0);
            glVertex3f(0, h/2, 0); glVertex3f(w, h/2, 0);
            
            glVertex3f(1, 1, 0); glVertex3f(w-1, 1, 0);
            glVertex3f(1, h-1, 0); glVertex3f(w-1, h-1, 0);

            glVertex3f(1, 1, 0); glVertex3f(1, h-1, 0);
            glVertex3f(w-1, 1, 0); glVertex3f(w-1, h-1, 0);
            
            glVertex3f(1, 1, 0); glVertex3f(w-1, h-1, 0);
            glVertex3f(w-1, 1, 0); glVertex3f(1, h-1, 0);
            
            glEnd();
        }
    }
    
    if(testApp::bShowInfo){
        ofPushMatrix();{
            ofTranslate(0,0);
            ofSetColor(ofColor(255,255,255)-testApp::bg);
            ofFill();
            ofRect(0, 0, w, 30);
            ofSetColor(testApp::bg);
            int y = 23;
            ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()),20,y);
            ofDrawBitmapString("picto num: " + ofToString(gpuPicto::totalPicto), 200, y);
            
            ofDrawBitmapString("Frame num: " + ofToString(ofGetFrameNum()), 400, y);
        }ofPopMatrix();
    }
    
    renderFBO.end();
    ofPopStyle();
    
    pastOffset = offset;
    


}

void gpuPictoString::drawForPdf(){
    
    
    ofDisableAlphaBlending();
    ofDisableSmoothing();
    
    int w = testApp::getW();
    int h = testApp::getH();
    
    int total = gpuPicto::totalPicto;
    
    ofTexture& postex = posPingPong.dst->getTextureReference(); // pos.x, y, alpha
    int texw = postex.getWidth();
    int texh = postex.getHeight();

    ofFloatPixels pix;
    pix.allocate(texw, texh, OF_PIXELS_RGBA);
    postex.readToPixels(pix);

    ofFloatPixels iconPrmPix;
    iconPrmPix.allocate(texw, texh, OF_PIXELS_RGBA);
    iconPrmTex.readToPixels(iconPrmPix);
    
    float iconSize = prm.iconSize*h/128.0;


//    warning
//
//    Color of svg parts will go white when we push style.
//    So we do not push style here
//
//    glPushAttrib(GL_ALL_ATTRIB_BITS);
//    ofPushStyle();

    ofSetRectMode(OF_RECTMODE_CENTER);
  
    
    int count = 0;
    for(int i=0; i<texh; i++){        
        for(int j=0; j<texw; j++){
            
            // position, alpha
            ofFloatColor posa = pix.getColor(j, i);
            float x = posa.r * w;
            float y = posa.g * h;
            float a = posa.b * 255.0;
    
            // iconPrm
            ofFloatColor iconPrm = iconPrmPix.getColor(j, i);
            int colorType = iconPrm.r*100;
            int iconType = iconPrm.g*100;
            if(iconType>numIcon-1){ iconType = numIcon-1; }
            
            // draw svg
            ofPushMatrix();
            ofTranslate(x, y, 0);
            ofScale(iconSize, iconSize, 1);
            
            ofFill();
            ofSetColor(colors[colorType].r, colors[colorType].g, colors[colorType].b, a);
            svg[iconType].draw();
            ofPopMatrix();

            
            count++;
            if(count>=total) break;
        }
        
        if(count>=total) break;
    }

    
    ofSetRectMode(OF_RECTMODE_CORNER);
    
//    ofPopStyle();
//    glPopAttrib();

    
}

void gpuPictoString::draw(){

    ofPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    int w = testApp::getW();
    int h = testApp::getH();

    ofSetColor(255,255,255);
    ofNoFill();
    
    {
        renderFBO.draw(0,0);
        //texadv.draw();
    }
    
    if(testApp::getDebugDraw()){
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
    }
    
    glPopAttrib();
    ofPopStyle();

}



void gpuPictoString::drawPreview(){

    ofPushStyle();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
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
    
    glPopAttrib();
    ofPopStyle();
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
    
    bClear = false;
    clearFrame = -1;
    cout << "Clear All" << endl;
    
}

void gpuPictoString::resize(float w, float h){
    // windowing
    //if(renderFBO.isAllocated()){
        renderFBO.allocate(w, h, GL_RGB32F);
        renderFBO.begin();
        ofClear(0,0,0,0);
        renderFBO.end();
    //}

    
    //texId = renderFBO.getTextureReference().getTextureData().textureID;
    //texadv.setExternalTexture(w, h, texId);
    //texadv.setPoints(0, 0, w, 0, w, h, 0, h);
    
    
    /*
    texadv.setPoints(61, 131,
                     1223, 17,
                     1185, 613,
                     72, 447);
    */
    
    //printf("\n\n renderFbo texture id = %i\n\n", texId);
}

#include "ofxXmlSettings.h"
void gpuPictoString::savePresets(string path, vector<PrmData> prms){
    ofxXmlSettings xml;

    for(int i=0; i<prms.size(); i++){
        PrmData p = prms[i];
        xml.addTag("preset");
        xml.pushTag("preset", i);{
            xml.addValue("message", p.message);
            xml.addValue("fontSize", ofToString(p.fontSize));
            xml.addValue("lineHeight", ofToString(p.lineHeight));
            xml.addValue("letterSpacing", ofToString(p.letterSpacing));
            xml.addValue("fontRandomness", ofToString(p.fontRandomness));
            xml.addValue("iconSize", ofToString(p.iconSize));
            xml.addValue("iconDensity", ofToString(p.iconDensity));
            xml.addValue("speed", ofToString(p.speed));
            xml.addValue("accel", ofToString(p.accel));
            xml.addValue("vibration", ofToString(p.vibration));
            xml.addValue("holdTime", ofToString(p.holdTime));
        }xml.popTag();
    }
    xml.saveFile(path);
}

vector<PrmData> gpuPictoString::loadPresets(string path){
    vector<PrmData> ps;
    ps.reserve(16);

    ofxXmlSettings xml;
    
    bool ok = xml.loadFile(path);
    if(ok){
        int n= xml.getNumTags("preset");
        for(int i=0; i<n; i++){
            PrmData p;
            xml.pushTag("preset", i);{
                p.message       = xml.getValue("message", "ERROR");
                p.fontSize      = xml.getValue("fontSize", 0.2);
                p.lineHeight    = xml.getValue("lineHeight", 1.15);
                p.letterSpacing = xml.getValue("letterSpacing", 1.0);
                p.fontRandomness= xml.getValue("fontRandomness", 0.0);
                p.iconSize      = xml.getValue("iconSize", 0.03);
                p.iconDensity   = xml.getValue("iconDensity", 0.05);
                p.speed         = xml.getValue("speed", 10.0);
                p.accel         = xml.getValue("accel", 15.0);
                p.vibration     = xml.getValue("vibration", 0.0);
                p.holdTime      = xml.getValue("holdTime", 1000);
            }xml.popTag();
            ps.push_back(p);
        }
    }
    return ps;
}



bool gpuPictoString::clearCheck(){
    if(clearFrame < 0)
        return false;
    
    float now = ofGetFrameNum();
    
    if(!bClear){
        if(clearFrame<=now){
            bClear = true;
            cout << "clear frame !!" << endl;
            return true;
        }
    }
    
    return false;
}

bool gpuPictoString::shouldStartNextCheck(){
    if(shouldStartNextFrame < 0)
        return false;
        
    float now = ofGetFrameNum();
    
    if(!bShouldStartNext){
        if(shouldStartNextFrame<=now){
            bShouldStartNext = true;
            cout << "next start frame !!" << endl;
            return true;
        }
    }
    return false;
}

