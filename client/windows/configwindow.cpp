#include "configwindow.h"

#include "mainwindow.h"
#include "ui_configwindow.h"
#include "../audio/BGM.h"

int ConfigWindow::music_state = 100;
int ConfigWindow::sound_state = 100;
bool ConfigWindow::music_on = 1;
bool ConfigWindow::sound_on = 1;
ConfigWindow::ConfigWindow(QWidget *parent) : BaseWindow(parent), ui(new Ui::ConfigWindow) {
    ui->setupUi(this);
    updateState();
}

ConfigWindow::~ConfigWindow() { delete ui; }

void ConfigWindow::updateState(){
    if (sound_on) {
        ui->soundSlider->setEnabled(true);
        ui->btnSound->setStyleSheet(
            "QPushButton#btnSound{border-image:url(:/images/configwindow/"
            "setting_sound_on.png)}"
            "QPushButton::pressed#btnSound{border-image:url(:/images/configwindow/"
            "setting_sound_on-.png)}");
    } else {
        ui->soundSlider->setEnabled(false);
        ui->btnSound->setStyleSheet(
            "QPushButton#btnSound{border-image:url(:/images/configwindow/"
            "setting_sound_off.png)}"
            "QPushButton::pressed#btnSound{border-image:url(:/images/configwindow/"
            "setting_sound_off-.png)}");
    }
    if (music_on) {
        ui->musicSlider->setEnabled(true);
        ui->btnMusic->setStyleSheet(
            "QPushButton#btnMusic{border-image:url(:/images/configwindow/"
            "setting_music_on.png)}"
            "QPushButton::pressed#btnMusic{border-image:url(:/images/configwindow/"
            "setting_music_on-.png)}");
    } else {
        ui->musicSlider->setEnabled(false);
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
    BGM::GetInstance()->PlayLabel();
    changeWindow(new MainWindow());
}

void ConfigWindow::on_btnMusic_clicked() {
    if(music_on == 1){

        BGM::GetInstance()->stopAllMusic = true;
        BGM::GetInstance()->StopBgm1();
    } else{

        BGM::GetInstance()->stopAllMusic = false;
        BGM::GetInstance()->PlayBgm1();
    }
    music_on ^= 1;
    updateState();
}

void ConfigWindow::on_btnSound_clicked() {
    if(sound_on == 1){

        BGM::GetInstance()->stopAllSound = true;

    } else{

        BGM::GetInstance()->stopAllSound = false;
    }
    sound_on ^= 1;
    updateState();
}

void ConfigWindow::on_musicSlider_valueChanged(int value)
{
    qDebug() << value << '\n';
    music_state=value;
    BGM::GetInstance()->ModifyBgm1(value);
    BGM::GetInstance()->ModifyBgm2(value);
    BGM::GetInstance()->allMusicPercent = value;
}


void ConfigWindow::on_soundSlider_valueChanged(int value)
{
    sound_state=value;
    BGM::GetInstance()->ModifyLabel(value);
    BGM::GetInstance()->ModifyOpen(value);
    BGM::GetInstance()->ModifySkill(value);
    BGM::GetInstance()->allSoundPercent = value;
}

