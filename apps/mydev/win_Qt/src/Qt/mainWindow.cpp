#include "ofMain.h"
#include "ofxQtGLWidget.h"
#include "mainWidget.h"
#include "mainWindow.h"

#include <QtWidgets>
#include <QDesktopWidget>

MainWindow * MainWindow::instance = NULL;

MainWindow::MainWindow(){
    move(0,0);
    setBaseSize(1280,720);

    QGridLayout * lay = new QGridLayout();
    setLayout(lay);

    mainGLWidget = new mainWidget();

    lay->addWidget(mainGLWidget);
    lay->setMargin(0);
    setLayout(lay);

    setWindowTitle("main");
}


