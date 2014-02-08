#include "subWidget.h"

void subWidget::setup(){}

void subWidget::update(){}

void subWidget::draw(){
    ofPushStyle();
    ofPushMatrix();

    ofBackground(20, 20, 120);
    ofSetColor(0, 20, 200);

//    ofTranslate(0, 100);
    ofRect(100, 100, 50, 50);

    ofPopMatrix();
    ofPopStyle();
}

void subWidget::exit(){}
