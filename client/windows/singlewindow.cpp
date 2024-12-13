#include "singlewindow.h"
#include "mainwindow.h"
#include "ui_singlewindow.h"
#include "pausewindow.h"
#include "../audio/BGM.h"
#include <QAction>
#include <QBitmap>
#include <QDebug>
#include <QFont>
#include <QFontDatabase>
#include <QPicture>
#include <QPixmap>
#include <ctime>
#include <iostream>
#include <random>
const QPoint hypercube_size(550, 550);
const QPoint opengl_up_left(25, 25);
const QPoint opengl_down_right = opengl_up_left + QPoint(hypercube_size.x(), hypercube_size.y());
const int TITLE_HEIGHT = 30;

SingleWindow::SingleWindow(QWidget *parent)
    : BaseWindow(parent), ui(new Ui::SingleWindow) {
    ui->setupUi(this);

}

SingleWindow::~SingleWindow() {

}

void SingleWindow::RefreshTimeLabel() {

}

void SingleWindow::InitBoard() {

}

void SingleWindow::Release1() { ui->skill1_button->setIcon(QIcon(":/images/SingleWindow/1.png")); }
void SingleWindow::Release2() { ui->skill2_button->setIcon(QIcon(":/images/SingleWindow/2.png")); }
void SingleWindow::Release3() { ui->skill3_button->setIcon(QIcon(":/images/SingleWindow/3.png")); }

void SingleWindow::mousePressEvent(QMouseEvent *event) {

}

void SingleWindow::mouseMoveEvent(QMouseEvent *event) {

}

void SingleWindow::mouseReleaseEvent(QMouseEvent *event) {

}

void SingleWindow::keyPressEvent(QKeyEvent *e) {

}

void SingleWindow::getDifficulty(QString data) {

}

void SingleWindow::on_btnReturn_clicked() {
    MainWindow *mw = new MainWindow();
    mw->move(this->pos().x(), this->pos().y());
    mw->show();
    delay(20);
    // timer_flush_score_and_left_time_bar_->stop();
    this->close();
}

void SingleWindow::on_skill1_button_clicked() {

}

void SingleWindow::on_skill2_button_clicked() {

}

void SingleWindow::on_skill3_button_clicked() {

}

void SingleWindow::on_pause_button_clicked() {
    PauseWindow *pw = new PauseWindow(this);
    pw->setGeometry(0,0,1280,720);
    pw->show();
    connect(pw, &PauseWindow::exitwindow, this, [this]{
        BGM::GetInstance()->StopBgm2();
        changeWindow(new MainWindow());
    });
}

void SingleWindow::on_hint_button_clicked() {
    //board->ShowHint(true);  // 提示
}

void SingleWindow::on_hint_button_pressed() { ui->hint_button->setIcon(QIcon(":/images/SingleWindow/5-.png")); }

void SingleWindow::on_hint_button_released() { ui->hint_button->setIcon(QIcon(":/images/SingleWindow/5.png")); }

void SingleWindow::on_btnReturn_pressed() {  }

void SingleWindow::on_btnReturn_released() { }
