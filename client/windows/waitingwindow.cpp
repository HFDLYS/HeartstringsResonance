#include "waitingwindow.h"
#include "ui_waitingwindow.h"
#include "gamewindow.h"
#include <QTime>
#include <QTimer>
#include "mainwindow.h"
#include "../audio/audiomanager.h"
#include "../config/globalconfig.h"


WaitingWindow::WaitingWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WaitingWindow)
{
    ui->setupUi(this);
    QString part1 = ":/images/singlewindow/background";
    QString picid = QString::number(GlobalConfig::getInstance().getPicStyle() + 1);
    QString root = part1.append(picid)+ ".png";
    ui->widget->setStyleSheet(
        QString("#widget{border-image:url(%1)}").arg(root)
        );
    ui->label1->setVisible(false);
    ui->label2->setVisible(false);
    ui->label3->setVisible(false);
    timer = new QTimer(this);
    connect((GameWindow*)parent,&GameWindow::wait,this,[=](int a){
        ui->playernum->setText("排队人数:"+QString::number(a));
    });
    connect(timer, &QTimer::timeout, this, &WaitingWindow::updateLabels);
    timer->start(1000);
    connect((GameWindow*)parent,&GameWindow::gameStart,this,[&]{
        hide();
    });
}

WaitingWindow::~WaitingWindow()
{
    delete ui;
}
void WaitingWindow::keyPressEvent(QKeyEvent *e) {
    qDebug()<<"waiting";
    if(e->key()==Qt::Key_Escape){
        on_btnExit_clicked();
    }
}
void WaitingWindow::on_btnExit_clicked()
{
    emit closeGame();
}
void WaitingWindow::updateLabels()
{
    cnt++;


    switch (cnt) {
    case 1:
        ui->label1->setVisible(true);
        ui->label2->setVisible(false);
        ui->label3->setVisible(false);
        break;
    case 2:
        ui->label1->setVisible(true);
        ui->label2->setVisible(true);
        ui->label3->setVisible(false);
        break;
    case 3:
        ui->label1->setVisible(true);
        ui->label2->setVisible(true);
        ui->label3->setVisible(true);
        break;
    case 4:
        ui->label1->setVisible(false);
        ui->label2->setVisible(false);
        ui->label3->setVisible(false);
        break;
    }

    if (cnt == 4) {
        cnt = 0;
    }
}

