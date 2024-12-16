#include "rankwindow.h"

#include <QFont>
#include <QFontDatabase>

#include "mainwindow.h"
#include "ui_rankwindow.h"
#include "../audio/audiomanager.h"
RankWindow::RankWindow(QWidget *parent) : BaseWindow(parent), ui(new Ui::RankWindow) {
    ui->setupUi(this);
    // 固定窗口大小
    this->setFixedSize(1280, 720);
    // 去除自带的边框
    this->setWindowFlag(Qt::FramelessWindowHint);

}

RankWindow::~RankWindow() { delete ui; }

void RankWindow::on_btnReturn_clicked() {
    AudioManager::GetInstance()->PlayLabel();
    changeWindow(new MainWindow());
}
