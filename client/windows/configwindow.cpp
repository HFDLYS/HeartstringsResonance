#include "configwindow.h"

#include "mainwindow.h"
#include "ui_configwindow.h"
#include "../audio/audiomanager.h"

int ConfigWindow::music_state = 100;
int ConfigWindow::sound_state = 100;
bool ConfigWindow::music_on = 1;
bool ConfigWindow::sound_on = 1;
ConfigWindow::ConfigWindow(QWidget *parent) : BaseWindow(parent), ui(new Ui::ConfigWindow) {

    ui->setupUi(this);
    ui->BtnBgd1->setChecked(true);
    ui->BtnBod1->setChecked(true);
    ui->BtnMsc1->setChecked(true);
    ui->BtnDim1->setChecked(true);
    hideMsc();
    hideBod();
    hideBgd();
    showDim();

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
    AudioManager::GetInstance()->PlayLabel();
    changeWindow(new MainWindow());
}

void ConfigWindow::on_btnMusic_clicked() {
    if(music_on == 1){

        AudioManager::GetInstance()->stopAllMusic = true;
        AudioManager::GetInstance()->StopBgm1();
    } else{

        AudioManager::GetInstance()->stopAllMusic = false;
        AudioManager::GetInstance()->PlayBgm1();
    }
    music_on ^= 1;
    updateState();
}

void ConfigWindow::on_btnSound_clicked() {
    if(sound_on == 1){

        AudioManager::GetInstance()->stopAllSound = true;

    } else{

        AudioManager::GetInstance()->stopAllSound = false;
    }
    sound_on ^= 1;
    updateState();
}

void ConfigWindow::on_musicSlider_valueChanged(int value)
{
    qDebug() << value << '\n';
    music_state=value;
    AudioManager::GetInstance()->ModifyBgm1(value);
    AudioManager::GetInstance()->ModifyBgm2(value);
    AudioManager::GetInstance()->ModifyBgm3(value);
    AudioManager::GetInstance()->allMusicPercent = value;
}


void ConfigWindow::on_soundSlider_valueChanged(int value)
{
    sound_state=value;
    AudioManager::GetInstance()->ModifyLabel(value);
    AudioManager::GetInstance()->ModifyOpen(value);
    AudioManager::GetInstance()->ModifySkill(value);
    AudioManager::GetInstance()->allSoundPercent = value;
}




void ConfigWindow::hideDim(){
    ui->widgetDim->hide();
    ui->BtnDim1->setEnabled(false);
    ui->BtnDim2->setEnabled(false);
    ui->BtnDim3->setEnabled(false);
}

void ConfigWindow::hideMsc(){
    ui->widgetMsc->hide();
    ui->BtnMsc1->setEnabled(false);
    ui->BtnMsc2->setEnabled(false);
    ui->BtnMsc3->setEnabled(false);
}

void ConfigWindow::hideBod(){
    ui->widgetBod->hide();
    ui->BtnBod1->setEnabled(false);
    ui->BtnBod2->setEnabled(false);
    ui->BtnBod3->setEnabled(false);
}

void ConfigWindow::hideBgd(){
    ui->widgetBgd->hide();
    ui->BtnBgd1->setEnabled(false);
    ui->BtnBgd2->setEnabled(false);
    ui->BtnBgd3->setEnabled(false);
}

void ConfigWindow::showDim(){
    ui->widgetDim->show();
    ui->BtnDim1->setEnabled(true);
    ui->BtnDim2->setEnabled(true);
    ui->BtnDim3->setEnabled(true);
}

void ConfigWindow::showMsc(){
    ui->widgetMsc->show();
    ui->BtnMsc1->setEnabled(true);
    ui->BtnMsc2->setEnabled(true);
    ui->BtnMsc3->setEnabled(true);
}

void ConfigWindow::showBod(){
    ui->widgetBod->show();
    ui->BtnBod1->setEnabled(true);
    ui->BtnBod2->setEnabled(true);
    ui->BtnBod3->setEnabled(true);
}
void ConfigWindow::showBgd(){
    ui->widgetBgd->show();
    ui->BtnBgd1->setEnabled(true);
    ui->BtnBgd2->setEnabled(true);
    ui->BtnBgd3->setEnabled(true);
}

void ConfigWindow::on_BtnDia_clicked()
{
    showDim();
    hideMsc();
    hideBod();
    hideBgd();
}

void ConfigWindow::on_btnBgd_clicked()
{
    showBgd();
    hideMsc();
    hideBod();
    hideDim();
}


void ConfigWindow::on_BtnBod_clicked()
{
    showBod();
    hideMsc();
    hideBgd();
    hideDim();
}


void ConfigWindow::on_BtnMsc_clicked()
{
    showMsc();
    hideBgd();
    hideBod();
    hideDim();
}

