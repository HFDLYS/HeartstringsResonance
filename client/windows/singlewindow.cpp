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
const QPoint board_size(640, 640);
const QPoint opengl_up_left(200, 40);
const QPoint opengl_down_right = opengl_up_left + QPoint(board_size.x(), board_size.y());
const int TITLE_HEIGHT = 30;

SingleWindow::SingleWindow(QWidget *parent)
    : BaseWindow(parent), ui(new Ui::SingleWindow) {
    ui->setupUi(this);
    renderer_ = new Graphics::RenderManager(ui->controlWidget);
    renderer_->setFixedSize(board_size.x(), board_size.y());
    renderer_->setGeometry(opengl_up_left.x(), opengl_up_left.y(), renderer_->width(), renderer_->height());


}

SingleWindow::~SingleWindow() {

}

void SingleWindow::refreshTimeLabel() {

}

void SingleWindow::initBoard() {
    board = new Board();
    board->SetGemManager(renderer_->GetGemManager());
    board->initBoard();

}

void SingleWindow::Release1() { ui->skill1_button->setIcon(QIcon(":/images/SingleWindow/1.png")); }
void SingleWindow::Release2() { ui->skill2_button->setIcon(QIcon(":/images/SingleWindow/2.png")); }
void SingleWindow::Release3() { ui->skill3_button->setIcon(QIcon(":/images/SingleWindow/3.png")); }

void SingleWindow::mousePressEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();
    // std::cout << "mouse cliked on:" << x << " " << y << std::endl;
    if (event->y() < TITLE_HEIGHT) {
        last = event->globalPos();
    }
    // board->Clicked(x, y);
    if (x > opengl_up_left.x() && y > opengl_up_left.y() && x < opengl_down_right.x() && y < opengl_down_right.y()) {
        
        x -= opengl_up_left.x();
        y -= opengl_up_left.y();
        std::cout << "mouse cliked on:" << x << " " << y << std::endl;
        std::cout <<  x /(board_size.x() / 8)*8 + y/(board_size.y() / 8)+1 << '\n';
        int nd = x /(board_size.x() / 8)*8 + y/(board_size.y() / 8)+1;
        int nx = 8 * x/(board_size.x());
        int ny = 8 * y/(board_size.y());
        board->clicked(nx, ny);
    }
}

void SingleWindow::mouseMoveEvent(QMouseEvent *event) {

}

void SingleWindow::mouseReleaseEvent(QMouseEvent *event) {

}

void SingleWindow::keyPressEvent(QKeyEvent *e) {

}

void SingleWindow::startGame() {
    initBoard();
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
    BGM::GetInstance()->PlaySkill();
}

void SingleWindow::on_skill2_button_clicked() {
    BGM::GetInstance()->PlaySkill();
}

void SingleWindow::on_skill3_button_clicked() {
    BGM::GetInstance()->PlaySkill();
}

void SingleWindow::on_pause_button_clicked() {
    PauseWindow *pw = new PauseWindow(this);
    BGM::GetInstance()->PauseBgm2();
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
