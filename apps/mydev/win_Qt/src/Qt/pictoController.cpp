#include "pictoController.h"
#include "ui_pictocontroller.h"
#include "subWidget.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include <QtWidgets>
#include <QDesktopWidget>

pictoController::pictoController(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::pictoController){

    ui->setupUi(this);

    subwidget = new subWidget();

    subwidget->setWindowTitle("subWidget");
    ui->gridLayout_3->addWidget(subwidget, 0, 2, 1, 1);
}

pictoController::~pictoController(){
    delete ui;
}

void pictoController::on_quitButton_clicked(){
    qApp->quit();
}

void pictoController::on_actionOpen_triggered(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
                tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        QTextStream in(&file);
        //ui->textEdit->setText(in.readAll());
        file.close();
    }
}

void pictoController::on_actionSave_triggered(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
            tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            // error message
        } else {
            QTextStream stream(&file);
          //  stream << ui->textEdit->toPlainText();
            stream.flush();
            file.close();
        }
    }
}
