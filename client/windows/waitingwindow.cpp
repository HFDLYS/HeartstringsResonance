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
    emit closeGame();
}

