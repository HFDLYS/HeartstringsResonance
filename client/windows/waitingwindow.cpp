#include "waitingwindow.h"
#include "ui_waitingwindow.h"
#include "gamewindow.h"
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
