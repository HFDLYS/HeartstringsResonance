#include "configwindow.h"

#include "mainwindow.h"
#include "ui_configwindow.h"

int ConfigWindow::music_state = 30;
int ConfigWindow::sound_state = 30;
bool ConfigWindow::music_on = 1;
bool ConfigWindow::sound_on = 1;
ConfigWindow::ConfigWindow(QWidget *parent) : BaseWindow(parent), ui(new Ui::ConfigWindow) {
    ui->setupUi(this);
    updateState();
}

ConfigWindow::~ConfigWindow() { delete ui; }

void ConfigWindow::updateState(){
    if (sound_on) {
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
    if (music_on) {
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

void ConfigWindow::on_btnReturn_clicked() {
    //BGM::GetInstance()->PlayClose();
    changeWindow(new MainWindow());
}

void ConfigWindow::on_btnMusic_clicked() {
    music_on=!music_on;
    updateState();
}

void ConfigWindow::on_btnSound_clicked() {
    sound_on=!sound_on;
    updateState();
}

void ConfigWindow::on_musicSlider_valueChanged(int value)
{
    music_state=value;
}


void ConfigWindow::on_soundSlider_valueChanged(int value)
{
    sound_state=value;
}

