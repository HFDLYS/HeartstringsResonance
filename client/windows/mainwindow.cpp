#include "mainwindow.h"

#include <QFile>
#include <iostream>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : FrameLessWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    // 固定窗口大小
    this->setFixedSize(1280, 720);
    // 去除自带的边框
    this->setWindowFlag(Qt::FramelessWindowHint);
    msg = "solo";
    //BGM::GetInstance()->PlayBgm1();
}

MainWindow::~MainWindow() { delete ui; }
//通过开启其他窗口并关闭本窗口实现界面的改变.

void delay(int x) {
    QTime dieTime = QTime::currentTime().addMSecs(x);
    while (QTime::currentTime() < dieTime) QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

// 游戏开始
void MainWindow::on_btnGame_clicked() {
    //BGM::GetInstance()->StopBgm1();
    //GameWindow *gw = new GameWindow();
    //gw->move(this->pos().x(), this->pos().y());
    //BGM::GetInstance()->PlayBgm2();

    //connect(this, &MainWindow::sentDifficulty, gw, &GameWindow::getDifficulty);
    //gw->show();
    //std::cerr << "MainWindow::on_btnGame_clicked - GameWindow has showed." << std::endl;
    //emit sentDifficulty(msg);
    //delay(200);
    //this->close();
}

// 排行榜
void MainWindow::on_btnRank_clicked() {
    //BGM::GetInstance()->PlaySwitchType();
    //RankWindow *rw = new RankWindow();
    //rw->move(this->pos().x(), this->pos().y());
    //rw->show();
    //delay(200);
    //this->close();
}

// 设置
void MainWindow::on_btnConfig_clicked() {
    //BGM::GetInstance()->PlaySwitchType();
    //ConfigWindow *cw = new ConfigWindow();
    //cw->move(this->pos().x(), this->pos().y());
    //cw->show();
    //delay(200);
    //this->close();
}

// 关于
void MainWindow::on_btnAbout_clicked() {
    //BGM::GetInstance()->PlaySwitchType();
    //AboutWindow *aw = new AboutWindow();
    //aw->move(this->pos().x(), this->pos().y());
    //aw->show();
    //delay(200);
    //this->close();
}

void MainWindow::on_btnQuit_clicked() {
    //BGM::GetInstance()->PlayClose();
    this->close();
}

void MainWindow::on_btnGame_pressed() {}

void MainWindow::on_btnGame_released() {}

void MainWindow::on_btnRank_pressed() {}

void MainWindow::on_btnRank_released() {}

void MainWindow::on_btnConfig_pressed() {}

void MainWindow::on_btnConfig_released() {}

void MainWindow::on_rbtnSolo_clicked()
{
    msg = "solo";

    //ui->rbtnSolo->setStyleSheet("border-image:url(:/images/mainwindow/solo-.png)");
    //ui->rbtnMultiplayer->setStyleSheet("border-image:url(:/images/mainwindow/mul.png)");
}


void MainWindow::on_rbtnMultiplayer_clicked()
{
    msg = "mul";
    //ui->rbtnSolo->setStyleSheet("border-image:url(:/images/mainwindow/solo.png)");
    //ui->rbtnMultiplayer->setStyleSheet("border-image:url(:/images/mainwindow/mul-.png)");
}

