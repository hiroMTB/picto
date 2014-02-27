#include "pictoController.h"
#include "ui_pictoController.h"
#include "subWidget.h"
#include "testApp.h"
#include "gpuPictoString.h"

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
    QSizePolicy policy;
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    policy.setHorizontalPolicy(QSizePolicy::Fixed);
    subwidget->setSizePolicy(policy);
    subwidget->setFixedSize(385,216);
    ui->gridLayout_3->addWidget(subwidget, 0, 2, 1, 1);

    move(1300,0);

    //preset tab settings
    ui->tableWidget->setColumnWidth(0, 300);

    QFont font;
    font.setPointSize(7);
    font.setFamily("MS UI Gothic");
    font.setBold(true);
    ui->tableWidget->horizontalHeader()->setFont(font);
}

pictoController::~pictoController(){
    delete ui;
}


//void pictoController::on_actionOpen_triggered(){
//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
//                tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

//    if (!fileName.isEmpty()) {
//        QFile file(fileName);
//        if (!file.open(QIODevice::ReadOnly)) {
//            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
//            return;
//        }
//        QTextStream in(&file);
//        //ui->textEdit->setText(in.readAll());
//        file.close();
//    }
//}

//void pictoController::on_actionSave_triggered(){
//    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
//            tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

//    if (!fileName.isEmpty()) {
//        QFile file(fileName);
//        if (!file.open(QIODevice::WriteOnly)) {
//            // error message
//        } else {
//            QTextStream stream(&file);
//          //  stream << ui->textEdit->toPlainText();
//            stream.flush();
//            file.close();
//        }
//    }
//}



void pictoController::on_FontSize_slider_valueChanged(int value){
    gpuPictoString::prm.fontSize = value*0.01*1.119 + 0.001;
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

void pictoController::on_LineHeight_slider_valueChanged(int value){
    gpuPictoString::prm.lineHeight = value*0.01*2.0 + 1.0;
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}
void pictoController::on_LetterSpacing_slider_valueChanged(int value){
    gpuPictoString::prm.letterSpacing = value*0.01*0.9 + 0.1;
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

void pictoController::on_FontRandomness_slider_valueChanged(int value){
    gpuPictoString::prm.fontRandomness = value*0.01*0.1;
}

void pictoController::on_IconSize_slider_valueChanged(int value){
    gpuPictoString::prm.iconSize = value*0.01*0.049 + 0.001;
}

void pictoController::on_IconDensity_slider_valueChanged(int value){
    gpuPictoString::prm.iconDensity = value*0.01*0.19 + 0.01;
}

void pictoController::on_Speed_slider_valueChanged(int value){
    gpuPictoString::prm.speed = value*0.01*10.0 + 10.0;
}

void pictoController::on_Accel_slider_valueChanged(int value){
    gpuPictoString::prm.accel = value*0.01*10.0 + 10.0;
}

void pictoController::on_Vibration_slider_valueChanged(int value){
    gpuPictoString::prm.vibration = value*0.01*4.99 + 0.01;
}

void pictoController::on_HoldTime_slider_valueChanged(int value){
    gpuPictoString::prm.holdTime = value*0.01*9000 + 1000;
}

void pictoController::on_Message_box_textChanged(){
    gpuPictoString::prm.message = ui->Message_box->document()->toPlainText().toUtf8().constData();
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

void pictoController::on_Preview_button_clicked(){
    gpuPictoString::prm.message = ui->Message_box->document()->toPlainText().toUtf8().constData();
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

void pictoController::on_ClearAnimation_button_clicked(){
    testApp::getInstance()->clearAll();
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

void pictoController::on_StartAnimation_button_clicked(){
    gpuPictoString::prm.message = ui->Message_box->document()->toPlainText().toUtf8().constData();
    testApp::getInstance()->gps->makeAnimation();
}


void pictoController::on_Fullscreen_check_stateChanged(int arg1){
    testApp::setFullscreen(arg1);
}

void pictoController::on_Black_check_stateChanged(int arg1){
    testApp::setBlack(arg1);
}

void pictoController::on_DebugDraw_check_stateChanged(int arg1){
    testApp::setDebugDraw(arg1);
}

void pictoController::on_ShowInfoBar_check_stateChanged(int arg1){
    testApp::setShowInfo(arg1);
}

void pictoController::on_WallMapAdjust_check_stateChanged(int arg1){
    testApp::setWallMapMouseAdjust(arg1);
}

void pictoController::on_TestPicture_check_stateChanged(int arg1){
    testApp::setTestPicture(arg1);
}

void pictoController::on_Capture_button_clicked(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(), tr("PDF Files (*.pdf);;C++ Files (*.cpp *.h)"));
    string path = fileName.toUtf8().constData();
    if (!fileName.isEmpty()) {
        cout << path << endl;
        testApp::bCap = true;
        testApp::pdfCapturePath = path;
    }
}

void pictoController::on_AddPreset_button_clicked(){
    const int rowNum= ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(rowNum+1);
    ui->tableWidget->setItem(rowNum,1,new QTableWidgetItem("aaa"));
}

void pictoController::on_RemovePreset_button_clicked(){
    QModelIndexList indexes = ui->tableWidget->selectionModel()->selectedRows();
    for(int i=0; i<indexes.count(); i++){
        QModelIndex index = indexes.at(i);
        ui->tableWidget->removeRow(index.row());
    }
}

void pictoController::on_pushButton_6_clicked()
{

}

void pictoController::on_pushButton_7_clicked()
{

}

void pictoController::on_PlayPreset_button_clicked()
{

}

void pictoController::on_checkBox_5_stateChanged(int arg1)
{

}

void pictoController::on_checkBox_6_stateChanged(int arg1)
{

}
