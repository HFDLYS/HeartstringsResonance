#include "pausewindow.h"
#include "mainwindow.h"
#include "ui_pausewindow.h"

PauseWindow::PauseWindow(QWidget *parent) : QWidget(parent),ui(new Ui::PauseWindow){
    ui->setupUi(this);
    // 固定窗口大小
}


PauseWindow::~PauseWindow() {  }

void PauseWindow::on_btnReturn_clicked(){

}

void PauseWindow::on_btnConit_clicked(){

}



