#include "mainWidget.h"
#include "testApp.h"

mainWidget::mainWidget(){
    cout << "mainWidget constractor" << endl;
    app = testApp::getInstance();
}

void mainWidget::setup(){
    if(app)app->setup();
    setWindowShape(1280, 720);
//    setWindowPosition(0,0);
}

void mainWidget::update(){
    if(app)app->update();
}

void mainWidget::draw(){
    ofPushView();
    int w = getWidth();
    int h = getHeight();
    glViewport(0,0, w, h);
    ofSetupScreenPerspective(getWidth(), getHeight());

    if(app)app->draw();

    ofPopView();
}


void mainWidget::keyPressed(int key){
    if(app)app->keyPressed(key);
}

void mainWidget::keyReleased(int key){
    if(app)app->keyReleased(key);
}

void mainWidget::mouseMoved(int x, int y){
    if(app)app->mouseMoved(x, y);
}

void mainWidget::mouseDragged(int x, int y, int button){
    if(app)app->mouseDragged(x, y, button);
}

void mainWidget::mousePressed(int x, int y, int button){
    if(app)app->mousePressed(x, y, button);
}

void mainWidget::mouseReleased(int x, int y, int button){
    if(app)app->mouseReleased(x, y, button);
}

void mainWidget::windowResized(int w, int h){
    if(app)app->windowResized(w, h);
}

void mainWidget::exit(){
    if(app)app->exit();
}
