#include "configwindow.h"

#include "mainwindow.h"
#include "ui_configwindow.h"

int ConfigWindow::music_state = 30;
int ConfigWindow::sound_state = 30;
bool ConfigWindow::music_on = 1;
bool ConfigWindow::sound_on = 1;
ConfigWindow::ConfigWindow(QWidget *parent) : FrameLessWindow(parent), ui(new Ui::ConfigWindow) {
    ui->setupUi(this);
    // 固定窗口大小
    this->setFixedSize(1280, 720);
    // 去除自带的边框
    this->setWindowFlag(Qt::FramelessWindowHint);
    if (sound_on == 1) {
        ui->btnSound->setStyleSheet(
            "QPushButton#btnSound{border-image:url(:/images/configwindow/"
            "setting_sound_on.png)}"
            "QPushButton::pressed#btnSound{border-image:url(:/images/configwindow/"
            "setting_sound_on-.png)}");
    } else {
        ui->btnSound->setStyleSheet(
            "QPushButton#btnSound{border-image:url(:/images/configwindow/"
            "setting_sound_off.png)}"
            "QPushButton::pressed#btnSound{border-image:url(:/images/configwindow/"
            "setting_sound_off-.png)}");
    }
    if (music_on == 1) {
        ui->btnMusic->setStyleSheet(
            "QPushButton#btnMusic{border-image:url(:/images/configwindow/"
            "setting_music_on.png)}"
            "QPushButton::pressed#btnMusic{border-image:url(:/images/configwindow/"
            "setting_music_on-.png)}");
    } else {
        ui->btnMusic->setStyleSheet(
            "QPushButton#btnMusic{border-image:url(:/images/configwindow/"
            "setting_music_off.png)}"
            "QPushButton::pressed#btnMusic{border-image:url(:/images/configwindow/"
            "setting_music_off-.png)}");
    }
    ui->musicSlider->setValue(music_state);
    ui->soundSlider->setValue(sound_state);
}

ConfigWindow::~ConfigWindow() { delete ui; }

void ConfigWindow::on_btnReturn_clicked() {
    //BGM::GetInstance()->PlayClose();
    MainWindow *mw = new MainWindow();
    mw->move(this->pos().x(), this->pos().y());
    mw->show();
    delay(20);
    this->close();
}

void ConfigWindow::on_btnMusic_clicked() {
    music_on=!music_on;
    if (music_on == 1) {
        ui->btnMusic->setStyleSheet(
            "QPushButton#btnMusic{border-image:url(:/images/configwindow/"
            "setting_music_on.png)}"
            "QPushButton::pressed#btnMusic{border-image:url(:/images/configwindow/"
            "setting_music_on-.png)}");
    } else {
        ui->btnMusic->setStyleSheet(
            "QPushButton#btnMusic{border-image:url(:/images/configwindow/"
            "setting_music_off.png)}"
            "QPushButton::pressed#btnMusic{border-image:url(:/images/configwindow/"
            "setting_music_off-.png)}");
    }
}

void ConfigWindow::on_btnSound_clicked() {
    sound_on=!sound_on;
    if (sound_on == 1) {
        ui->btnSound->setStyleSheet(
            "QPushButton#btnSound{border-image:url(:/images/configwindow/"
            "setting_sound_on.png)}"
            "QPushButton::pressed#btnSound{border-image:url(:/images/configwindow/"
            "setting_sound_on-.png)}");
    } else {
        ui->btnSound->setStyleSheet(
            "QPushButton#btnSound{border-image:url(:/images/configwindow/"
            "setting_sound_off.png)}"
            "QPushButton::pressed#btnSound{border-image:url(:/images/configwindow/"
            "setting_sound_off-.png)}");
    }
}

void ConfigWindow::on_musicSlider_valueChanged(int value)
{
    music_state=value;
}


void ConfigWindow::on_soundSlider_valueChanged(int value)
{
    sound_state=value;
}

