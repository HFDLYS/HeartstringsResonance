#include "mainwindow.h"
#include <QFile>
#include <QDialog>
#include <iostream>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QInputDialog>
#include <climits>
#include "../component/connectdialog.h"
#include <QFormLayout>
#include "ui_mainwindow.h"
#include "aboutwindow.h"
#include "configwindow.h"
#include "rankwindow.h"
#include "gamewindow.h"
#include "singlewindow.h"
#include "../audio/audiomanager.h"


MainWindow::MainWindow(QWidget *parent) : BaseWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    AudioManager::GetInstance()->PlayBgm1();
    qDebug()<< "test" << player.username;
}

MainWindow::~MainWindow() { delete ui; }
//通过开启其他窗口并关闭本窗口实现界面的改变.

// 单人游戏开始
void MainWindow::on_rbtnSolo_clicked() {
    AudioManager::GetInstance()->PlayOpen();
    AudioManager::GetInstance()->StopBgm1();
    AudioManager::GetInstance()->PlayBgm2();
    SingleWindow *sw = new SingleWindow(0);
    changeWindow(sw);
    sw->startGame();
}


void MainWindow::on_rbtnMultiplayer_clicked(){
    AudioManager::GetInstance()->PlayOpen();
    AudioManager::GetInstance()->StopBgm1();
    AudioManager::GetInstance()->PlayBgm3();
    GameWindow *gw = new GameWindow(this);
    changeWindow(gw);
}

// 排行榜
void MainWindow::on_btnRank_clicked() {
    AudioManager::GetInstance()->PlayOpen();
    changeWindow(new RankWindow());
}

// 设置
void MainWindow::on_btnConfig_clicked() {
    AudioManager::GetInstance()->PlayOpen();
    changeWindow(new ConfigWindow());
}

// 关于
void MainWindow::on_btnAbout_clicked() {
    AudioManager::GetInstance()->PlayLabel();
    changeWindow(new AboutWindow());
}
//退出
void MainWindow::on_btnQuit_clicked() {
    AudioManager::GetInstance()->PlayLabel();
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
    QFont fontBtn;
    fontBtn.setPointSize(18);
    label->setFont(font);        // 应用新字体
    confirmButton->setFont(fontBtn);
    cancelButton->setFont(fontBtn);
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
QString MainWindow::getip(){
    return ip;
}
QString MainWindow::getport(){
    return port;
}

