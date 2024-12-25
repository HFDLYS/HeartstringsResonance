#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include "basewindow.h"

// 设置窗口类

namespace Ui {
class ConfigWindow;
}


class ConfigWindow : public BaseWindow {
    Q_OBJECT

public:
    explicit ConfigWindow(QWidget *parent = nullptr);
    ~ConfigWindow();

private slots:
    void on_btnReturn_clicked();

    void on_btnMusic_clicked();

    void on_btnSound_clicked();

    void on_musicSlider_valueChanged(int value);

    void on_soundSlider_valueChanged(int value);

    void on_BtnDia_clicked();

    void on_btnBgd_clicked();

    void on_BtnBod_clicked();

    void on_BtnMsc_clicked();

    void on_BtnRecharge_clicked();

    void on_btnBack_clicked();

private:
    Ui::ConfigWindow *ui;
    static bool music_on;
    static bool sound_on;
    static int music_state;
    static int sound_state;
    void updateState();

    void hideDim();
    void hideBod();
    void hideMsc();
    void hideBgd();
    void showDim();
    void showBod();
    void showMsc();
    void showBgd();
};

#endif  // CONFIGWINDOW_H
