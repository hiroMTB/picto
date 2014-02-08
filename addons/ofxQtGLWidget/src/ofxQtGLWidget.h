#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "ofMain.h"
#include "ofAppBaseWindow.h"

#include <QGLWidget>

class ofxQtGLWidget : public QGLWidget,  public ofAppBaseWindow
{
    Q_OBJECT

public:
    ofxQtGLWidget(QWidget *parent = 0);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:
    void timerLoop();

// signals:

protected:

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *){};

    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

    void focusInEvent(QFocusEvent *){};
    void focusOutEvent(QFocusEvent *){};
    void enterEvent(QEvent *){};
    void leaveEvent(QEvent *){};
//    void paintEvent(QPaintEvent *){};     // this make oF bitmap font buggy
    void moveEvent(QMoveEvent *){};
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *);

private:

    int mouseX, mouseY;
    int frameNum;
    float frameRate;

public:

    void setOpenGL(int w, int h, int screenMode){}

    int getFrameNum();
    float getFrameRate();
    double getLastFrameTime();
    int getWidth();
    int getHeight();
    ofPoint getWindowSize();
    string getWindowTitle();
    void setFrameRate(float f);
    void setFullscreen(bool b);
    void toggleFullscreen();
    void hideCursor();
    void showCursor();
    void setWindowPosition(int x, int y);
    void setWindowShape(int width, int height);
    void setWindowTitle(string s);

    // should be overwirte from testApp.cpp
    virtual ~ofxQtGLWidget(){};
    virtual void setup(){};
    virtual void update(){};
    virtual void draw(){};
    virtual void exit(){};
    virtual void windowResized(int x, int y){};

    virtual void mouseMoved(int x, int y){};
    virtual void mousePressed(int x, int y, int button){};
    virtual void mouseReleased(int x, int y, int button){};
    virtual void mouseDragged(int x, int y, int button){};
    virtual void keyPressed(int key){};
    virtual void keyReleased(int key){};

};

#endif
