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
    AudioManager::GetInstance()->StopBgm3();
    MainWindow *flw = new MainWindow();
    flw->move(this->pos().x(), this->pos().y());
    flw->show();
    QTime dieTime = QTime::currentTime().addMSecs(20);
    while (QTime::currentTime() < dieTime) QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    this->close();
}

