#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include "basewindow.h"
#include "../config/globalconfig.h"
#include "../graphics/rendermanager.h"
#include <QLayout>

// 设置窗口类

namespace Ui {
class ConfigWindow;
}


class ConfigWindow : public BaseWindow {
    Q_OBJECT

public:
    explicit ConfigWindow(QWidget *parent = nullptr);
    ~ConfigWindow();
    void showBoard();
protected:
    void keyPressEvent(QKeyEvent *e);
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
    static int style_flag;
    static bool music_on;
    static bool sound_on;
    static int music_state;
    static int sound_state;
    void updateState();
    Graphics::RenderManager *renderer_;

    void showDim();
    void showBod();
    void showMsc();
    void showBgd();
};

#endif  // CONFIGWINDOW_H
