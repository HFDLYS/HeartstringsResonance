#include "resultwindow.h"
#include "ui_resultwindow.h"
#include "mainwindow.h"
#include "../audio/audiomanager.h"
ResultWindow::ResultWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ResultWindow)
{
    ui->setupUi(this);

}

void ResultWindow::on_btnReturn_clicked(){
    emit exitwindow();
}

ResultWindow::~ResultWindow()
{
    delete ui;
}
