#include "pictoController.h"
#include "ui_pictoController.h"
#include "subWidget.h"
#include "testApp.h"
#include "gpuPictoString.h"
#include "mainWindow.h"
#include "mainWidget.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include <QtWidgets>
#include <QDesktopWidget>

pictoController * pictoController::instance = NULL;

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

    //ui->tableWidget->horizontalHeaderItem(1)->setFlags(Qt::ItemFlag::);
}

pictoController::~pictoController(){
    delete ui;
}

void pictoController::on_FontSize_slider_valueChanged(int value){
    gpuPictoString::prm.fontSize = toFontSize(value);
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

void pictoController::on_LineHeight_slider_valueChanged(int value){
    gpuPictoString::prm.lineHeight = toLineHeight(value);
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}
void pictoController::on_LetterSpacing_slider_valueChanged(int value){
    gpuPictoString::prm.letterSpacing = toLetterSpacing(value);
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

void pictoController::on_FontRandomness_slider_valueChanged(int value){
    gpuPictoString::prm.fontRandomness = toFontRandomness(value);
}

void pictoController::on_IconSize_slider_valueChanged(int value){
    gpuPictoString::prm.iconSize = toIconSize(value);
}

void pictoController::on_IconDensity_slider_valueChanged(int value){
    gpuPictoString::prm.iconDensity = toIconDensity(value);
}

void pictoController::on_Speed_slider_valueChanged(int value){
    gpuPictoString::prm.speed = toSpeed(value);
}

void pictoController::on_Accel_slider_valueChanged(int value){
    gpuPictoString::prm.accel = toAccel(value);
}

void pictoController::on_Vibration_slider_valueChanged(int value){
    gpuPictoString::prm.vibration = toVibration(value);
}

void pictoController::on_HoldTime_slider_valueChanged(int value){
    gpuPictoString::prm.holdTime = toHoldTime(value);
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
    MainWindow::getInstance()->mainGLWidget->makeCurrent();
    testApp::getInstance()->clearAll();
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

void pictoController::on_StartAnimation_button_clicked(){
    gpuPictoString::prm.message = ui->Message_box->document()->toPlainText().toUtf8().constData();

    MainWindow::getInstance()->mainGLWidget->makeCurrent();
    testApp::getInstance()->gps->makeAnimation();
}

void pictoController::on_Fullscreen_check_stateChanged(int arg1){
    //testApp::setFullscreen(arg1);

    MainWindow::getInstance()->mainGLWidget->setFullscreen(arg1);
}

void pictoController::on_Black_check_stateChanged(int arg1){
    testApp::gprm.bBlack = arg1;
}

void pictoController::on_DebugDraw_check_stateChanged(int arg1){
    testApp::gprm.bDebugDraw = arg1;
}

void pictoController::on_ShowInfoBar_check_stateChanged(int arg1){
    testApp::gprm.bShowInfo = arg1;
}

void pictoController::on_WallMapAdjust_check_stateChanged(int arg1){
    testApp::gprm.bWallMapMouseAdjust = arg1;
}

void pictoController::on_TestPicture_check_stateChanged(int arg1){
    testApp::gprm.bTestPicture = arg1;
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

void pictoController::on_PlayPreset_button_clicked(){

    int row = ui->tableWidget->currentRow();
    if(row<0){
        row = 0;
    }
    setParameterFromPresetRow(row);
    //testApp::getInstance()->gps->bNeedUpdateCharPos;

    MainWindow::getInstance()->mainGLWidget->makeCurrent();
    testApp::getInstance()->makeAnimation();
}

void pictoController::on_Capture_button_clicked(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save PDF Capture File"), QString(), tr("PDF Files (*.pdf)"));
    string path = fileName.toUtf8().constData();
    if (!fileName.isEmpty()) {
        cout << path << endl;
        testApp::bCap = true;
        testApp::pdfCapturePath = path;
    }
}

void pictoController::on_SavePreset_button_clicked(){
    QString path = QFileDialog::getSaveFileName(this, tr("Save XML Preset File"), QString(), tr("XML Files (*.xml)"));
    if (!path.isEmpty()){
        int rowNum = ui->tableWidget->rowCount();
        vector<PrmData> prms;
        prms.reserve(rowNum);

        // iterate all row
        for(int i=0; i<rowNum; i++){
            prms.push_back(getRecordAtRow(i));
        }
        gpuPictoString::savePresets(path.toUtf8().constData(), prms);
    }
}

void pictoController::on_LoadPreset_button_clicked(){
    QString path = QFileDialog::getOpenFileName(this, tr("Open XML Preset File"), QString(), tr("XML Files (*.xml)"));

    if (!path.isEmpty()) {
        clearAllPreset();
        vector<PrmData> ps = gpuPictoString::loadPresets(path.toUtf8().constData());
        for(int i=0; i<ps.size(); i++){
            addPresetFromPrmData(ps[i]);
        }
    }
}

void pictoController::on_AutoPlayNext_check_stateChanged(int arg1){
    testApp::gprm.bAutoPlay = arg1;
}

void pictoController::on_Loop_check_stateChanged(int arg1){
    testApp::gprm.bLoop = arg1;
}

/*
 *   Return record data with filterling
 */
PrmData pictoController::getRecordAtRow(int r){
    QTableWidget * t = ui->tableWidget;

    PrmData prm;
    if(t->item(r, 0)) prm.message         = t->item(r, 0)->text().toUtf8().constData();
    if(t->item(r, 1)) prm.fontSize        = ofToFloat(t->item(r, 1)->text().toUtf8().constData());
    if(t->item(r, 2)) prm.lineHeight      = ofToFloat(t->item(r, 2)->text().toUtf8().constData());
    if(t->item(r, 3)) prm.letterSpacing   = ofToFloat(t->item(r, 3)->text().toUtf8().constData());
    if(t->item(r, 4)) prm.fontRandomness  = ofToFloat(t->item(r, 4)->text().toUtf8().constData());
    if(t->item(r, 5)) prm.iconSize        = ofToFloat(t->item(r, 5)->text().toUtf8().constData());
    if(t->item(r, 6)) prm.iconDensity     = ofToFloat(t->item(r, 6)->text().toUtf8().constData());
    if(t->item(r, 7)) prm.speed           = ofToFloat(t->item(r, 7)->text().toUtf8().constData());
    if(t->item(r, 8)) prm.accel           = ofToFloat(t->item(r, 8)->text().toUtf8().constData());
    if(t->item(r, 9))prm.vibration       = ofToFloat(t->item(r, 9)->text().toUtf8().constData());
    if(t->item(r, 10))prm.holdTime        = ofToFloat(t->item(r,10)->text().toUtf8().constData());
    return prm;
}

void pictoController::clearAllPreset(){
    int n = ui->tableWidget->rowCount();
    for(int i=0; i<n; i++){
        ui->tableWidget->removeRow(i);
    }
}

void pictoController::addPresetFromPrmData(PrmData p){
    QTableWidget * t = ui->tableWidget;
    const int rowNum= t->rowCount();
    t->setRowCount(rowNum+1);
    t->setItem(rowNum,0,new QTableWidgetItem(p.message.c_str()));
    t->setItem(rowNum,1,new QTableWidgetItem(ofToString(p.fontSize).c_str()));
    t->setItem(rowNum,2,new QTableWidgetItem(ofToString(p.lineHeight).c_str()));
    t->setItem(rowNum,3,new QTableWidgetItem(ofToString(p.letterSpacing).c_str()));
    t->setItem(rowNum,4,new QTableWidgetItem(ofToString(p.fontRandomness).c_str()));
    t->setItem(rowNum,5,new QTableWidgetItem(ofToString(p.iconSize).c_str()));
    t->setItem(rowNum,6,new QTableWidgetItem(ofToString(p.iconDensity).c_str()));
    t->setItem(rowNum,7,new QTableWidgetItem(ofToString(p.speed).c_str()));
    t->setItem(rowNum,8,new QTableWidgetItem(ofToString(p.accel).c_str()));
    t->setItem(rowNum,9,new QTableWidgetItem(ofToString(p.vibration).c_str()));
    t->setItem(rowNum,10,new QTableWidgetItem(ofToString(p.holdTime).c_str()));
}


void pictoController::setParameterFromPrmData(PrmData prm){
    ui->Message_box->setText(prm.message.c_str());
    ui->FontSize_slider->setValue(toFontSizeUI(prm.fontSize));
    ui->LineHeight_slider->setValue(toLineHeightUI(prm.lineHeight));
    ui->LetterSpacing_slider->setValue(toLetterSpacingUI(prm.letterSpacing));
    ui->FontRandomness_slider->setValue(toFontRandomnessUI(prm.fontRandomness));
    ui->IconSize_slider->setValue(toIconSizeUI(prm.iconSize));
    ui->IconDensity_slider->setValue(toIconDensityUI(prm.iconDensity));
    ui->Speed_slider->setValue(toSpeedUI(prm.speed));
    ui->Accel_slider->setValue(toAccelUI(prm.accel));
    ui->Vibration_slider->setValue(toVibrationUI(prm.vibration));
    ui->HoldTime_slider->setValue(toHoldTimeUI(prm.holdTime));

    gpuPictoString::prm = prm;
}

void pictoController::setParameterFromPresetRow(int row){
    PrmData prm = getRecordAtRow(row);
    setParameterFromPrmData(prm);
}

void pictoController::on_sendToPreset_button_clicked(){
    PrmData prm;
    prm.message         = ui->Message_box->document()->toPlainText().toUtf8().constData();
    prm.fontSize        = toFontSize(ui->FontSize_slider->value());
    prm.lineHeight      = toLineHeight(ui->LineHeight_slider->value());
    prm.letterSpacing   = toLetterSpacing(ui->LetterSpacing_slider->value());
    prm.fontRandomness  = toFontRandomness(ui->FontRandomness_slider->value());
    prm.iconSize        = toIconSize(ui->IconSize_slider->value());
    prm.iconDensity     = toIconDensity(ui->IconDensity_slider->value());
    prm.speed           = toSpeed(ui->Speed_slider->value());
    prm.accel           = toAccel(ui->Accel_slider->value());
    prm.vibration       = toVibration(ui->Vibration_slider->value());
    prm.holdTime        = toHoldTime(ui->HoldTime_slider->value());

    addPresetFromPrmData(prm);
}

float pictoController::toFontSize(int value){       return (float)value*0.01*1.199 + 0.001; }
float pictoController::toLineHeight(int value){     return (float)value*0.01*2.0 + 1.0; }
float pictoController::toLetterSpacing(int value){  return (float)value*0.01*1.9 + 0.1; }
float pictoController::toFontRandomness(int value){ return (float)value*0.01*0.1; }
float pictoController::toIconSize(int value){       return (float)value*0.01*0.049 + 0.001; }
float pictoController::toIconDensity(int value){    return (float)value*0.01*0.19 + 0.01; }

float pictoController::toSpeed(int value){          return (float)value*0.01*10.0 + 10.0;}
float pictoController::toAccel(int value){          return (float)value*0.01*10.0 + 10.0;}

float pictoController::toVibration(int value){      return (float)value*0.01*4.99 + 0.01;}
int   pictoController::toHoldTime(int value){       return (float)value*0.01*9000 + 1000;}

int pictoController::toFontSizeUI(float value){     return (value-0.001)/0.01/1.199; }
int pictoController::toLineHeightUI(float value){   return (value-1.0)/0.01/2.0; }
int pictoController::toLetterSpacingUI(float value){return (value-0.1)/0.01/1.9; }
int pictoController::toFontRandomnessUI(float value){return value/0.01/0.1;}
int pictoController::toIconSizeUI(float value){     return (value-0.001)/0.01/0.049; }
int pictoController::toIconDensityUI(float value){  return (value-0.01)/0.01/0.19; }
int pictoController::toSpeedUI(float value){        return (value-10.0)/0.01/10.0; }
int pictoController::toAccelUI(float value){        return (value-10.0)/0.01/10.0; }
int pictoController::toVibrationUI(float value){    return (value-0.01)/0.01/4.99; }
int pictoController::toHoldTimeUI(float value){     return (value-1000.0)/0.01/9000.0; }
int pictoController::toColorUI(float value){        return value/255.0 * 100.0;}

void pictoController::startNextAnimation(){

    int row = ui->tableWidget->currentRow();
    if(row>=0){
        row++;
    }else{
        row = 0;
    }
    // out of list
    if(row >= ui->tableWidget->rowCount()){

        if(testApp::gprm.bLoop){
            row=0;
        }else{
           cout << "end sequence" << endl;
           testApp::finishStartNextAnimation();

           //switch off
           return;
        }
    }

    ui->tableWidget->selectRow(row);

    MainWindow::getInstance()->mainGLWidget->makeCurrent();
    testApp::getInstance()->makeAnimation();
    testApp::finishStartNextAnimation();
}

void pictoController::on_BackgroundColorRed_slider_valueChanged(int value){
    testApp::gprm.bg.r = value*0.01*255.0;
}

void pictoController::on_BackgroundColorGreen_slider_valueChanged(int value){
    testApp::gprm.bg.g = value*0.01*255.0;
}


void pictoController::on_BackgroundColorBlue_slider_valueChanged(int value){
    testApp::gprm.bg.b = value*0.01*255.0;
}


void pictoController::closeEvent(QCloseEvent *){
    MainWindow::getInstance()->mainGLWidget->close();
    MainWindow::getInstance()->close();
    subwidget->close();

}

void pictoController::setGlobalParam(testApp::GlobalPrm gprm){
    ui->Black_check->setChecked(gprm.bBlack);
    ui->DebugDraw_check->setChecked(gprm.bDebugDraw);
    ui->ShowInfoBar_check->setChecked(gprm.bShowInfo);
    ui->WallMapAdjust_check->setChecked(gprm.bWallMapMouseAdjust);
    ui->TestPicture_check->setChecked(gprm.bTestPicture);
    ui->AutoPlayNext_check->setChecked(gprm.bAutoPlay);
    ui->Loop_check->setChecked(gprm.bLoop);

    ui->BackgroundColorRed_slider->setValue(toColorUI(gprm.bg.r));
    ui->BackgroundColorGreen_slider->setValue(toColorUI(gprm.bg.g));
    ui->BackgroundColorBlue_slider->setValue(toColorUI(gprm.bg.b));
}

void pictoController::createPopupError(string title, string description){
//    QErrorMessage errorMessage;
//    errorMessage.showMessage(s.c_str());
//    errorMessage.exec();
    QMessageBox::critical(NULL, title.c_str(), description.c_str());
}

void pictoController::createPopupInfo(string title, string description){
    QMessageBox::information(NULL, "adfadf", "adfdsf");
}

