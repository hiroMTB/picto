#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class mainWidget;
class testApp;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();
    mainWidget * mw;
};

#endif // MAINGLWINDOW_H
