#include "aboutwindow.h"
#include "mainwindow.h"
#include "ui_aboutwindow.h"
#include "../audio/audiomanager.h"
AboutWindow::AboutWindow(QWidget *parent) : BaseWindow(parent), ui(new Ui::AboutWindow) {
    ui->setupUi(this);
}

AboutWindow::~AboutWindow() { delete ui; }

void AboutWindow::on_btnReturn_clicked() {
    AudioManager::GetInstance()->PlayLabel();
    changeWindow(new MainWindow());
}
