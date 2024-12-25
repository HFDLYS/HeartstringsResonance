#include "configwindow.h"

#include "mainwindow.h"
#include "ui_configwindow.h"
#include "../audio/audiomanager.h"

int ConfigWindow::music_state = 100;
int ConfigWindow::sound_state = 100;
bool ConfigWindow::music_on = 1;
bool ConfigWindow::sound_on = 1;
int ConfigWindow::style_flag = 0;
ConfigWindow::ConfigWindow(QWidget *parent) : BaseWindow(parent), ui(new Ui::ConfigWindow) {
    ui->setupUi(this);
    ui->widgetQR->hide();
    if (style_flag == 0) {
        showDim();
    } else if (style_flag == 1) {
        showMsc();
    } else if (style_flag == 2) {
        showBod();
    } else if (style_flag == 3) {
        showBgd();
    }
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    renderer_ = new Graphics::RenderManager(this);
    renderer_->setFixedSize(240, 240);
    renderer_->setGeometry(840, 270, renderer_->width(), renderer_->height());
    auto a=connect(ui->listWidget, &QListWidget::itemChanged, [=](QListWidgetItem *currentItem) {
        if (currentItem->checkState() == Qt::Checked) {
        const int count = ui->listWidget->count();
        for (int i = 0; i < count; ++i) {
            QListWidgetItem *item = ui->listWidget->item(i);
            if (item != currentItem && item->checkState() == Qt::Checked) {
                item->setCheckState(Qt::Unchecked);
            }
        }
    }
    });
    connections.push_back(a);
    updateState();
}
ConfigWindow::~ConfigWindow() { delete ui; }

void ConfigWindow::showBoard() {
    renderer_->Demo();
}

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
    AudioManager::GetInstance()->ModifyMatch1(value);
    AudioManager::GetInstance()->ModifyMatch2(value);
    AudioManager::GetInstance()->ModifyMatch3(value);
    AudioManager::GetInstance()->allSoundPercent = value;
}





void ConfigWindow::showDim(){
    style_flag = 0;
    ui->listWidget->clear();
    QVector<QString> dim = {"和弦魔法", "动物乐园"};
    for (auto &i : dim) {
        QListWidgetItem *item = new QListWidgetItem(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        item->setCheckState(Qt::Unchecked);
        item->setSizeHint(QSize(0, 50));
        ui->listWidget->addItem(item);
    }
}

void ConfigWindow::showMsc(){
    style_flag = 1;
    ui->listWidget->clear();
    QVector<QString> msc = {"DokiDoki", "哈雪大冒险", "FairyTale"};
    for (auto &i : msc) {
        QListWidgetItem *item = new QListWidgetItem(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        item->setCheckState(Qt::Unchecked);
        item->setSizeHint(QSize(0, 50));
        ui->listWidget->addItem(item);
    }
}

void ConfigWindow::showBod(){
    style_flag = 2;
    ui->listWidget->clear();
    QVector<QString> bod = {"浩瀚心海", "魔法之时", "时间之尘", "心环相耀"};
    for (auto &i : bod) {
        QListWidgetItem *item = new QListWidgetItem(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        item->setCheckState(Qt::Unchecked);
        item->setSizeHint(QSize(0, 50));
        ui->listWidget->addItem(item);
    }
}
void ConfigWindow::showBgd(){
    style_flag = 3;
    ui->listWidget->clear();
    QVector<QString> bgd = {"心海相连", "背景2", "背景3", "背景4", "背景5"};
    for (auto &i : bgd) {
        QListWidgetItem *item = new QListWidgetItem(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        item->setCheckState(Qt::Unchecked);
        item->setSizeHint(QSize(0, 50));
        ui->listWidget->addItem(item);
    }
}

void ConfigWindow::on_BtnDia_clicked()
{
    showDim();
}

void ConfigWindow::on_btnBgd_clicked()
{
    showBgd();
}

void ConfigWindow::on_BtnBod_clicked()
{
    showBod();
}


void ConfigWindow::on_BtnMsc_clicked()
{
    showMsc();
}



void ConfigWindow::on_BtnRecharge_clicked()
{
    ui->widgetQR->show();
}


void ConfigWindow::on_btnBack_clicked()
{
    ui->widgetQR->hide();
    if (style_flag == 0) {
        for (int i = 0; i < ui->listWidget->count(); i++) {
            if (ui->listWidget->item(i)->checkState() == Qt::Checked) {
                GlobalConfig::getInstance().setGemStyle(i);
                break;
            }
        }
        // renderer_->Demo();
    } else if (style_flag == 1) {
        for (int i = 0; i < ui->listWidget->count(); i++) {
            if (ui->listWidget->item(i)->checkState() == Qt::Checked) {
                GlobalConfig::getInstance().setMusicStyle(i);
                break;
            }
        }
    } else if (style_flag == 2) {
        for (int i = 0; i < ui->listWidget->count(); i++) {
            if (ui->listWidget->item(i)->checkState() == Qt::Checked) {
                GlobalConfig::getInstance().setBoardStyle(i);
                break;
            }
        }
        // renderer_->Demo();
    } else if (style_flag == 3) {
        for (int i = 0; i < ui->listWidget->count(); i++) {
            if (ui->listWidget->item(i)->checkState() == Qt::Checked) {
                GlobalConfig::getInstance().setPicStyle(i);
                break;
            }
        }
    }
    ConfigWindow *cw = new ConfigWindow();
    changeWindow(cw);
    cw->showBoard();
}

