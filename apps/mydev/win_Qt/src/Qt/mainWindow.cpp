#include "ofMain.h"
#include "ofxQtGLWidget.h"
#include "mainWidget.h"
#include "mainWindow.h"

#include <QtWidgets>
#include <QDesktopWidget>


MainWindow::MainWindow(){
    QGridLayout * lay = new QGridLayout();
    setLayout(lay);

    mw = new mainWidget();

    lay->addWidget(mw);
    setLayout(lay);

    setWindowTitle("main");
}

