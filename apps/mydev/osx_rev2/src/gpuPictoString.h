//
//  gpuPictoString.h
//  osx_picto
//
//  Created by matobahiroshi on 2013/08/30.
//
//

#pragma once

#include "ofMain.h"
#include "pingPongBuffer.h"
#include "testApp.h"
#include "ofTextureAdv.h"

struct PrmData {
public:
    string message;
    float fontSize, lineHeight, letterSpacing, fontRandomness, iconSize, iconDensity, speed, accel, vibration;
    PrmData(string m="", float f1=0, float f2=0, float f3=0, float f4=0, float f5=0, float f6=0, float f7=0, float f8=0, float f9=0);
    PrmData(const PrmData& rhs);
    const PrmData& operator=(const PrmData& rhs);    
};

class gpuPictoChar;

class gpuPictoString{
  
public:
    gpuPictoString();
    ~gpuPictoString(){};

    void setup();
    void update();
    void draw();
    void drawForPdf();
    void drawPreview();
    
    vector<ofVec3f> calcCharPos();
    void makeAnimation();
    void clearAll();
    
    void setFinalTarget(int index, const ofVec2f& p);
    void setspringPrm(ofVec3f * pm, int num);
    void setIconPrm(ofVec3f * pm,  int num);
    
    
    ofShader updateTarget;
    ofShader updateVel;
    ofShader updatePos;
    ofShader updateRender;

    pingPongBuffer velPingPong;
    pingPongBuffer posPingPong;

    // prm
    bool bNeedUpdateTarget;
    bool bNeedUpdateIconPrm;
    bool needUpdateSpringPrm;
    
    float * finalTargetPosData;  // [0]=x, [1]=y, [2]=0
    float * iconPrmData;    // [0]=iconType, [1]=colorType, [2]=0
    float * springPrmData;  // [0]=K, [1]=minSpeed, [2]=maxSpeed
    float * randomData;
    float * velData;
    float * posData;
    
    ofTexture finalTargetTex;
    ofTexture iconPrmTex;
    ofTexture springPrmTex;
    ofTexture randomTex;
    
    ofFbo renderFBO;
    
    ofImage * img;
    ofImage img128, img64, img32, img16, img8, img4, img2, img1;
    
    float timeStep;
    float particleSize;
    int numParticles;
    int textureRes;
    float width, height;
    float imgSize;
    
public:
    typedef vector<gpuPictoChar*> GPICTO_STR;
    typedef GPICTO_STR::iterator GPICTO_STR_ITR;
    
    GPICTO_STR gpchars;
    
    static PrmData prm;
    static void setPrm(const PrmData &p){ prm = p; }
        
    ofTrueTypeFont font;
    float getFontScale(){ return prm.fontSize*testApp::getW() / 476.0; }
    

    vector<float> finalTargets;
    
    ofVec2f offset, pastOffset;
    ofVec2f offsetVel;
    
    void resize(float w, float h);
    
    bool bNeedUpdateCharPos;
    vector<ofVec3f> charPosList;
    
    static string alphabet;
    
    // svg
    ofxSVG svg[44];
    static const ofColor colors[5];
    
    
    static void savePresets(string path, vector<PrmData>);
    static vector<PrmData> loadPresets(string path);
    

    void resizeIcon(int h);
    
    //ofTextureAdv texadv;
    //GLuint texId;
};


