#include "mainWidget.h"

void mainWidget::setup(){
    //cout << "mainWidget::setup " << getWindowTitle() << endl;
    //ofHideCursor();
    //ofSetFullscreen(true);

    //setFrameRate(60);
    //setWindowShape(1200,600);
}

void mainWidget::update(){
    //cout << "mainWidget::update " << getWindowTitle()<< endl;
}

int px = 0;
int py = 0;
char  c = 'd';

void mainWidget::draw(){
    //cout << "mainWidget::draw" << getWindowTitle() << endl;

    ofPushStyle();
    ofPushMatrix();

    ofBackground(200, 200, 2);
    ofSetColor(200, 0,ofRandom(0, 255));
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofRect(px, py, 300, 300);

    ofDrawBitmapString("frameNum = "+ofToString(ofGetFrameNum()), 0, 10);
    ofDrawBitmapString("key = " + ofToString(c), 0, 30);

    ofPopMatrix();
    ofPopStyle();
}


void mainWidget::keyPressed(int key){
    c = (char)key;
}

void mainWidget::keyReleased(int key){}
void mainWidget::mouseMoved(int x, int y){}

void mainWidget::mouseDragged(int x, int y, int button){}
void mainWidget::mousePressed(int x, int y, int button){
    px = x;
    py = y;
}
void mainWidget::mouseReleased(int x, int y, int button){}
void mainWidget::windowResized(int w, int h){}
void mainWidget::exit(){}
