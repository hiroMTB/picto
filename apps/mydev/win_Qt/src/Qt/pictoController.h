#ifndef PICTO_CONTROLLER_H
#define PICTO_CONTROLLER_H

#include <QMainWindow>

class subWidget;

namespace Ui {
class pictoController;
}

class pictoController: public QMainWindow
{
    Q_OBJECT

public:
    explicit pictoController(QWidget *parent = 0);
    ~pictoController();

private slots:
    void on_quitButton_clicked();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

private:
    Ui::pictoController *ui;

    subWidget * subwidget;
};

#endif // PICTO_CONTROLLER_H
