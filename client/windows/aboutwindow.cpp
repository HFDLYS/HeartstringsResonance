#include "aboutwindow.h"
#include "mainwindow.h"
#include "ui_aboutwindow.h"
#include "../audio/BGM.h"
AboutWindow::AboutWindow(QWidget *parent) : BaseWindow(parent), ui(new Ui::AboutWindow) {
    ui->setupUi(this);
}

AboutWindow::~AboutWindow() { delete ui; }

void AboutWindow::on_btnReturn_clicked() {
    BGM::GetInstance()->PlayLabel();
    changeWindow(new MainWindow());
}
