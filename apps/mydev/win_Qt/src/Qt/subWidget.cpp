#include "subWidget.h"
#include "testApp.h"

void subWidget::setup(){
    app = testApp::getInstance();
}

void subWidget::update(){}

void subWidget::draw(){
    int w = getWidth();
    int h = getHeight();

    ofPushView();

    glViewport(0,0, w, h);
    ofSetupScreenPerspective(w, h);

    ofPushStyle();
    ofPushMatrix();

    app->drawPreview();

    ofPopMatrix();
    ofPopStyle();

    ofPopView();
}

void subWidget::exit(){}
