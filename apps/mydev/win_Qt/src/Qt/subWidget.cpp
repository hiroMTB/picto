#include "subWidget.h"
#include "testApp.h"

void subWidget::setup(){
    app = testApp::getInstance();
}

void subWidget::update(){}

void subWidget::draw(){
//    cout << "subWidget::draw()" << endl;
    int w = getWidth();
    int h = getHeight();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    ofPushStyle();
    ofPushView();
    ofPushMatrix();

    glViewport(0,0, w, h);
    ofSetupScreenPerspective(w, h);
    app->drawPreview();

    ofPopMatrix();
    ofPopView();
    ofPopStyle();
    glPopAttrib();
}

void subWidget::exit(){}
