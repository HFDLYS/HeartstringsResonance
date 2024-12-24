#include "waitingwindow.h"
#include "ui_waitingwindow.h"
#include "gamewindow.h"
#include <QTime>
#include <QTimer>
#include "mainwindow.h"
#include "../audio/audiomanager.h"
WaitingWindow::WaitingWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WaitingWindow)
{
    ui->setupUi(this);
    ui->label1->setVisible(false);
    ui->label2->setVisible(false);
    ui->label3->setVisible(false);
    timer = new QTimer(this);
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

void WaitingWindow::on_btnExit_clicked()
{
    emit closeGame();
}
void WaitingWindow::updateLabels()
{
    cnt++;  // 每秒增加计数器

    // 根据计数器的值显示不同的 QLabel
    switch (cnt) {
    case 1:
        ui->label1->setVisible(true); // 显示第一个
        ui->label2->setVisible(false);
        ui->label3->setVisible(false);
        break;
    case 2:
        ui->label1->setVisible(true);
        ui->label2->setVisible(true); // 显示第一个和第二个
        ui->label3->setVisible(false);
        break;
    case 3:
        ui->label1->setVisible(true);
        ui->label2->setVisible(true);
        ui->label3->setVisible(true); // 显示第一个、第二个和第三个
        break;
    case 4:
        ui->label1->setVisible(false);
        ui->label2->setVisible(false);
        ui->label3->setVisible(false); // 隐藏所有标签
        break;
    }

    // 如果超过4秒，重新从第 1 秒开始
    if (cnt == 4) {
        cnt = 0;
    }
}

