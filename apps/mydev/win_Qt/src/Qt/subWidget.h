#ifndef SUBWIDGET_H
#define SUBWIDGET_H

#include "ofxQtGLWidget.h"

class testApp;

class subWidget : public ofxQtGLWidget
{

    testApp * app;

public:
    subWidget(){};

    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int k){};
    void keyReleased(int k){};

    void mouseMoved(int x, int y){};
    void mousePressed(int x, int y, int button){};
    void mouseReleased(int x, int y, int button){};
    void mouseDragged(int x, int y, int button){};


    void windowResized(int w, int h){};
};

#endif // SUBWIDGET_H
