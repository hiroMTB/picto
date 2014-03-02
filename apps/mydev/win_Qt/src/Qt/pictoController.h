#ifndef PICTO_CONTROLLER_H
#define PICTO_CONTROLLER_H

#include "testApp.h"
#include <QMainWindow>

class subWidget;
class PrmData;

namespace Ui {
class pictoController;
}

class pictoController: public QMainWindow
{
    Q_OBJECT

private:
    static pictoController * instance;
    explicit pictoController(QWidget *parent = 0);

public:
    ~pictoController();

    void startNextAnimation();

    static pictoController * getInstance(){ return instance; }
    static void init(QWidget * parent=0){ if(!instance) instance = new pictoController(parent); }

    void closeEvent(QCloseEvent *);

private slots:

    void on_Message_box_textChanged();

    // page1 buttons
    void on_Preview_button_clicked();
    void on_ClearAnimation_button_clicked();
    void on_StartAnimation_button_clicked();

    // page1 sliders
    void on_FontSize_slider_valueChanged(int value);
    void on_LineHeight_slider_valueChanged(int value);
    void on_LetterSpacing_slider_valueChanged(int value);
    void on_FontRandomness_slider_valueChanged(int value);
    void on_IconSize_slider_valueChanged(int value);
    void on_IconDensity_slider_valueChanged(int value);
    void on_Speed_slider_valueChanged(int value);
    void on_Accel_slider_valueChanged(int value);
    void on_Vibration_slider_valueChanged(int value);
    void on_HoldTime_slider_valueChanged(int value);

    void on_Fullscreen_check_stateChanged(int arg1);
    void on_Black_check_stateChanged(int arg1);
    void on_DebugDraw_check_stateChanged(int arg1);
    void on_ShowInfoBar_check_stateChanged(int arg1);
    void on_WallMapAdjust_check_stateChanged(int arg1);
    void on_TestPicture_check_stateChanged(int arg1);
    void on_Capture_button_clicked();
    void on_AddPreset_button_clicked();
    void on_RemovePreset_button_clicked();
    void on_PlayPreset_button_clicked();
    void on_SavePreset_button_clicked();
    void on_LoadPreset_button_clicked();
    void on_AutoPlayNext_check_stateChanged(int arg1);
    void on_Loop_check_stateChanged(int arg1);
    void on_sendToPreset_button_clicked();
    void on_BackgroundColorRed_slider_valueChanged(int value);
    void on_BackgroundColorGreen_slider_valueChanged(int value);
    void on_BackgroundColorBlue_slider_valueChanged(int value);


public:

    // helper func
    PrmData getRecordAtRow(int r);
    void clearAllPreset();
    void addPresetFromPrmData(PrmData p);
    void setParameterFromPrmData(PrmData p);
    void setParameterFromPresetRow(int row);

    void setGlobalParam(testApp::GlobalPrm gprm);

private:
    Ui::pictoController *ui;

    subWidget * subwidget;


    // GUI value to PrmData value converter
    // Qt GUI value is 0-99 int
    float toFontSize(int value);
    float toLineHeight(int value);
    float toLetterSpacing(int value);
    float toFontRandomness(int value);
    float toIconSize(int value);
    float toIconDensity(int value);
    float toSpeed(int value);
    float toAccel(int value);
    float toVibration(int value);
    int   toHoldTime(int value);

    // PrmData value to GUI value converter
    int toFontSizeUI(float value);
    int toLineHeightUI(float value);
    int toLetterSpacingUI(float value);
    int toFontRandomnessUI(float value);
    int toIconSizeUI(float value);
    int toIconDensityUI(float value);
    int toSpeedUI(float value);
    int toAccelUI(float value);
    int toVibrationUI(float value);
    int toHoldTimeUI(float value);
    int toColorUI(float color);

};

#endif // PICTO_CONTROLLER_H
