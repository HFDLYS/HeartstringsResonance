#include "mainwindow.h"

#include <QFile>
#include <QDialog>
#include <iostream>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "ui_mainwindow.h"
#include "aboutwindow.h"
#include "configwindow.h"
#include "rankwindow.h"
#include "gamewindow.h"


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

// 单人游戏开始
void MainWindow::on_rbtnSolo_clicked() {
    msg = "solo";
    //BGM::GetInstance()->StopBgm1();
    GameWindow *gw = new GameWindow();
    gw->move(this->pos().x(), this->pos().y());
    //BGM::GetInstance()->PlayBgm2();

    //connect(this, &MainWindow::sentDifficulty, gw, &GameWindow::getDifficulty);
    gw->show();
    //std::cerr << "MainWindow::on_btnGame_clicked - GameWindow has showed." << std::endl;
    //emit sentDifficulty(msg);
    delay(200);
    this->close();
}

// 多人游戏开始
void MainWindow::on_rbtnMultiplayer_clicked(){
    msg = "muti";
}

// 排行榜
void MainWindow::on_btnRank_clicked() {
    //BGM::GetInstance()->PlaySwitchType();
    RankWindow *rw = new RankWindow();
    rw->move(this->pos().x(), this->pos().y());
    rw->show();
    delay(200);
    this->close();
}

// 设置
void MainWindow::on_btnConfig_clicked() {
    //BGM::GetInstance()->PlaySwitchType();
    ConfigWindow *cw = new ConfigWindow();
    cw->move(this->pos().x(), this->pos().y());
    cw->show();
    delay(50);
    this->close();
}

// 关于
void MainWindow::on_btnAbout_clicked() {
    //BGM::GetInstance()->PlaySwitchType();
    AboutWindow *aw = new AboutWindow();
    aw->move(this->pos().x(), this->pos().y());
    aw->show();
    delay(200);
    this->close();
}
//退出
void MainWindow::on_btnQuit_clicked() {
    //BGM::GetInstance()->PlayClose();
    QDialog *dialog = new QDialog(this);
    dialog->setFixedSize(400, 170);
    dialog->setWindowTitle("是否退出游戏？");
    //dialog->setWindowFlags(Qt::FramelessWindowHint);
    QHBoxLayout *layout = new QHBoxLayout(dialog);
    QPushButton *confirmButton = new QPushButton("确认", dialog);
    QPushButton *cancelButton = new QPushButton("取消", dialog);
    layout->addWidget(confirmButton);
    layout->addWidget(cancelButton);
    QVBoxLayout *layout2 = new QVBoxLayout(dialog);
    QWidget *tmp = new QWidget(this);
    tmp->setLayout(layout);
    QLabel *label = new QLabel("是否退出游戏?");
    QFont font;
    font.setBold(true);
    font.setPointSize(24);
    font.setFamily("幼圆");
    label->setFont(font);        // 应用新字体
    label->setAlignment(Qt::AlignCenter);
    layout2->addWidget(label);
    layout2->addWidget(tmp);
    dialog->setLayout(layout2);
    connect(confirmButton, &QPushButton::clicked, dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);
    if (dialog->exec() == QDialog::Accepted) {
        this->close();
    }
}

void MainWindow::on_btnRank_pressed() {}

void MainWindow::on_btnRank_released() {}

void MainWindow::on_btnConfig_pressed() {}

void MainWindow::on_btnConfig_released() {}


