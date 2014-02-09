#include "subWidget.h"
#include "testApp.h"

void subWidget::setup(){
    app = testApp::getInstance();
}

void subWidget::update(){}

void subWidget::draw(){
    ofPushStyle();
    ofPushMatrix();

    app->drawPreview();

    ofPopMatrix();
    ofPopStyle();
}

void subWidget::exit(){}
