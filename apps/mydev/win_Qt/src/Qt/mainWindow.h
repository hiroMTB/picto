#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class mainWidget;
class testApp;

class MainWindow : public QWidget
{
    Q_OBJECT

private:
    static MainWindow * instance;
    MainWindow();


public:
    static void init(){ if(!instance) instance = new MainWindow(); }
    static MainWindow * getInstance(){ return instance; }

    mainWidget * mainGLWidget;


};

#endif // MAINGLWINDOW_H
