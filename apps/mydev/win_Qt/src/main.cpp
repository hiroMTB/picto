#include "ofMain.h"
#include "mainWindow.h"
#include "pictoController.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QtWidgets>

#include "testApp.h"

int main(int argc, char *argv[]){

    testApp::init();

    QApplication app(argc, argv);

    pictoController::init();
    pictoController::getInstance()->show();

    MainWindow::init();
    MainWindow::getInstance()->show();

    return app.exec();
}


