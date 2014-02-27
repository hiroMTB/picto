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

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_PlayPreset_button_clicked();

    void on_checkBox_5_stateChanged(int arg1);

    void on_checkBox_6_stateChanged(int arg1);

private:
    Ui::pictoController *ui;

    subWidget * subwidget;
};

#endif // PICTO_CONTROLLER_H
