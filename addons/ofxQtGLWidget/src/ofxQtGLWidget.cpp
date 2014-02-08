#include "ofxQtGLWidget.h"

#include <QtWidgets>
#include <QtOpenGL>


#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

ofxQtGLWidget::ofxQtGLWidget(QWidget *parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
frameRate(60.0),
frameNum(0)
{
    //cout << "ofxQtWidget::ofxQtGLWidget  " << getWindowTitle() << ", " << this->width() << ", " << this->height() << endl;
    int w = this->width();
    int h = this->height();
    ofSetupOpenGL(this, w, h, OF_WINDOW);

    this->setup();

    QTimer * timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerLoop()));
    timer->start(1000.0/frameRate);
}

QSize ofxQtGLWidget::minimumSizeHint() const{
    return QSize(50, 50);
}

QSize ofxQtGLWidget::sizeHint() const{
    return QSize(720, 480);
}

void ofxQtGLWidget::initializeGL(){
    cout << "ofxQtGLWidget::initializeGL(): " << getWindowTitle() << endl;
}

void ofxQtGLWidget::paintGL(){
    int w = this->width();
    int h = this->height();

    //cout << "ofxQtGLWidget::paintGL(), " << getWindowTitle() << ", " << w << ", " <<  h << endl;

    ofSetupScreenPerspective(w, h);
    //ofSetupScreenOrtho(w, h);

    this->draw();
    frameNum++;
}

void ofxQtGLWidget::timerLoop(){
    //cout << "ofxQtGLWidget::timerLoop() " << getWindowTitle() << ", " << frameNum << endl;
    update();
    makeCurrent(); // need this?
    updateGL();
}

void ofxQtGLWidget::resizeGL(int width, int height){
//    cout << "ofxQtGLWidget::resizeGL() " << getWindowTitle() << ", " << width << ", " << height << endl;

    makeCurrent(); // need this?

    // * NOTICE *
    // ofViewport() calls ofGetWindowSize()
    // but ofGetWindowSize return wrong value when we use multiple window with different size.
    // so here we call glViewport directry.
    glViewport(0, 0, width, height);        // ofViewport(0,0, w, h);
    //updateGL();
}

void ofxQtGLWidget::mousePressEvent(QMouseEvent *e ){
    this->mousePressed(e->pos().x(), e->pos().y(), e->button());
    setFocus();
}

void ofxQtGLWidget::mouseMoveEvent(QMouseEvent * e){
    this->mouseMoved(e->pos().x(), e->pos().y());
}

void ofxQtGLWidget::mouseReleaseEvent(QMouseEvent * e){
    this->mouseReleased(e->pos().x(), e->pos().y(), e->button());
}

void ofxQtGLWidget::keyPressEvent(QKeyEvent * e){
    this->keyPressed(e->key());
}

void ofxQtGLWidget::keyReleaseEvent(QKeyEvent * e){
    this->keyReleased(e->key());
}

void ofxQtGLWidget::resizeEvent(QResizeEvent * e){
    //cout << "ofxQtGLWidget::resizeEvent " << getWindowTitle() << e->size().width() << ", " << e->size().height() << endl;
    resizeGL(e->size().width(), e->size().height());
    this->windowResized(e->size().width(), e->size().height());
}

void ofxQtGLWidget::closeEvent(QCloseEvent * e){
    this->exit();
}

int ofxQtGLWidget::getFrameNum(){
    return frameNum;
}

float ofxQtGLWidget::getFrameRate(){
    return frameRate;
}

void ofxQtGLWidget::setFrameRate(float f){
     frameRate = f;
}

int ofxQtGLWidget::getWidth(){
    return this->width();
}

int ofxQtGLWidget::getHeight(){
    return this->height();
}

ofPoint ofxQtGLWidget::getWindowSize(){
    return ofPoint(this->width(), this->height());
}


double ofxQtGLWidget::getLastFrameTime(){
    return -123;
}

string ofxQtGLWidget::getWindowTitle(){
    return this->window()->windowTitle().toUtf8().constData();
}

void ofxQtGLWidget::setFullscreen(bool b){
    if(b){
        this->window()->showFullScreen();
    }else{
        this->window()->showNormal();
    }
}

void ofxQtGLWidget::toggleFullscreen(){
    if(this->isFullScreen()) this->showNormal();
    else this->showFullScreen();
}

void ofxQtGLWidget::hideCursor(){
    this->setCursor(Qt::BlankCursor);
    //QApplication::setOverrideCursor( Qt::BlankCursor );
}
void ofxQtGLWidget::showCursor(){
    this->setCursor(Qt::ArrowCursor);
    //QApplication::setOverrideCursor( Qt::ArrowCursor);
}
void ofxQtGLWidget::setWindowPosition(int x, int y){
    this->window()->move(x, y);
}

void ofxQtGLWidget::setWindowShape(int width, int height){
    this->window()->resize(width, height);
}

void ofxQtGLWidget::setWindowTitle(string s){
    this->window()->setWindowTitle(s.c_str());
}
