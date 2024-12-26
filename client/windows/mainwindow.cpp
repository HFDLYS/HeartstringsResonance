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
    ui->selectwidget->hide();
    AudioManager::GetInstance()->PlayBgm1();
    qDebug()<< "test" << player.username;
    auto a=connect(ui->btnEasy, &QPushButton::clicked, this, &MainWindow::showEasy);
    connections.push_back(a);
    a=connect(ui->btnMedium, &QPushButton::clicked, this, &MainWindow::showMedium);
    connections.push_back(a);
    a=connect(ui->btnHard, &QPushButton::clicked, this, &MainWindow::showHard);
    connections.push_back(a);
}

MainWindow::~MainWindow() { delete ui; }
//通过开启其他窗口并关闭本窗口实现界面的改变.

// 单人游戏开始
void MainWindow::on_rbtnSolo_clicked() {
    ui->selectwidget->show();

}

//简单模式
void MainWindow::showEasy(){
    AudioManager::GetInstance()->PlayOpen();
    AudioManager::GetInstance()->StopBgm1();
    AudioManager::GetInstance()->PlayBgm2();
    SingleWindow *sw = new SingleWindow(0,1);
    changeWindow(sw);
    sw->startGame();
    ui->selectwidget->hide();
}


//中等模式
void MainWindow::showMedium(){
    if (player.pointSolo < 500) {
        QMessageBox::information(this, "提示", "前面的区域等你分数大于500再来探索吧");
        return;
    }
    AudioManager::GetInstance()->PlayOpen();
    AudioManager::GetInstance()->StopBgm1();
    AudioManager::GetInstance()->PlayBgm2();
    SingleWindow *sw = new SingleWindow(0,2);
    changeWindow(sw);
    sw->startGame();
    ui->selectwidget->hide();
}


//困难模式
void MainWindow::showHard(){
    if (player.pointSolo < 2000) {
        QMessageBox::information(this, "提示", "前面的区域等你分数大于2000再来探索吧");
        return;
    }
    AudioManager::GetInstance()->PlayOpen();
    AudioManager::GetInstance()->StopBgm1();
    AudioManager::GetInstance()->PlayBgm2();
    SingleWindow *sw = new SingleWindow(0,3);
    changeWindow(sw);
    sw->startGame();
    ui->selectwidget->hide();
}



// 多人游戏开始
void MainWindow::on_rbtnMultiplayer_clicked(){
    Gem::SetMaxType();
    AudioManager::GetInstance()->PlayOpen();
    AudioManager::GetInstance()->StopBgm1();
    AudioManager::GetInstance()->PlayBgm3();
    GameWindow *gw = new GameWindow();
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
    ConfigWindow *cw = new ConfigWindow();
    changeWindow(cw);
    cw->showBoard();
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
    auto a=connect(confirmButton, &QPushButton::clicked, dialog, &QDialog::accept);
    connections.push_back(a);
    a=connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);
    if (dialog->exec() == QDialog::Accepted) {
        this->close();
    }
    connections.push_back(a);
}
void MainWindow::keyPressEvent(QKeyEvent *e) {
    if(e->key()==Qt::Key_Escape){
        on_btnQuit_clicked();
    }
}
QString MainWindow::getip(){
    return ip;
}
QString MainWindow::getport(){
    return port;
}


void MainWindow::on_btnBack_clicked()
{
    ui->selectwidget->hide();
}

