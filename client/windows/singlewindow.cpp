#include "singlewindow.h"
#include "mainwindow.h"
#include "ui_singlewindow.h"
#include "pausewindow.h"
#include "../audio/audiomanager.h"
#include "resultwindow.h"
#include <QAction>
#include <QBitmap>
#include <QDebug>
#include <QFont>
#include <QFontDatabase>
#include <QPicture>
#include <QPixmap>
#include <ctime>
#include <iostream>
const QPoint board_size(640, 640);
const QPoint opengl_up_left(250, 40);
const QPoint opengl_down_right = opengl_up_left + QPoint(board_size.x(), board_size.y());
const int TITLE_HEIGHT = 30;
const int MAX_TIME=10;


SingleWindow::SingleWindow(int seed_,QWidget *parent)
    : BaseWindow(parent), ui(new Ui::SingleWindow), seed(seed_){
    ui->setupUi(this);
    ui->score->setText(QString::number(0));
    ui->skill1_button->setEnabled(1);
    ui->skill2_button->setEnabled(1);
    ui->skill3_button->setEnabled(1);
    ui->max_score->setText("史高: "+QString::number(player.pointSolo));
    if (player.pointSolo < 500) {
        ui->difficulty->setText("简单");
        max_gem_type = 5;
    } else if (player.pointSolo < 1000) {
        ui->difficulty->setText("中等");
        max_gem_type = 6;
    } else {
        ui->difficulty->setText("困难");
        max_gem_type = 8;
    }
    ui->cnt1->setText(QString::number(player.skill_1));
    ui->cnt2->setText(QString::number(player.skill_2));
    ui->cnt3->setText(QString::number(player.skill_3));
    qDebug()<<player.toJson();
    renderer_ = new Graphics::RenderManager(ui->controlWidget);
    renderer_->setFixedSize(board_size.x(), board_size.y());
    renderer_->setGeometry(opengl_up_left.x(), opengl_up_left.y(), renderer_->width(), renderer_->height());
}

SingleWindow::~SingleWindow() {

}

void SingleWindow::refreshTimeLabel() {

}

void SingleWindow::initBoard() {
    board = new Board(seed, max_gem_type);
    board->SetGemManager(renderer_->GetGemManager());
    board->initBoard();
}

void SingleWindow::Release1() { ui->skill1_button->setIcon(QIcon(":/images/SingleWindow/1.png")); }
void SingleWindow::Release2() { ui->skill2_button->setIcon(QIcon(":/images/SingleWindow/2.png")); }
void SingleWindow::Release3() { ui->skill3_button->setIcon(QIcon(":/images/SingleWindow/3.png")); }

void SingleWindow::mousePressEvent(QMouseEvent *event) {
    int x = event->position().x();
    int y = event->position().y();
    // std::cout << "mouse cliked on:" << x << " " << y << std::endl;
    if (y < TITLE_HEIGHT) {
        last = event->globalPosition().toPoint();
        qDebug()<<last;
        qDebug()<<this->x()<<this->y();
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

//void SingleWindow::mouseMoveEvent(QMouseEvent *event) {}

void SingleWindow::mouseReleaseEvent(QMouseEvent *event) {

}

void SingleWindow::keyPressEvent(QKeyEvent *e) {

}

void SingleWindow::startGame() {
    initBoard();
    ui->progressBar->setValue(MAX_TIME);
    timer=new QTimer(this);
    timer->start(1000);
    QObject::connect(timer, &QTimer::timeout,[&]{
        ui->progressBar->setValue(ui->progressBar->value()-1);
        ui->score->setText(QString::number(board->getScore(1) + board->getScore(2) + board->getScore(3) + board->getScore(4)));
        //时间到了
        if(!ui->progressBar->value()){
            timer->stop();
            delete timer;
            AudioManager::GetInstance()->StopBgm2();
            ResultWindow *rw = new ResultWindow(true,
                                                board->getScore(1) + board->getScore(2) + board->getScore(3) + board->getScore(4),
                                                board->getScore1(),
                                                board->getScore2(),
                                                board->getScore3(),
                                                board->getScore4(),
                                                board->getScore5(),
                                                this);
            rw->move(this->pos().x(), this->pos().y());
            rw->show();
            rw->showGem();
            connect(rw, &ResultWindow::exitwindow, this, [=]{
                rw->close();
                changeWindow(new MainWindow());
            });
        }
    });
}

void SingleWindow::on_skill1_button_clicked() {
    if(skill1_cnt>=player.skill_1)return;
    skill1_cnt++;
    ui->cnt1->setText(QString::number(player.skill_1-skill1_cnt));
    AudioManager::GetInstance()->PlaySkill();
    board->hint();
}

void SingleWindow::on_skill2_button_clicked() {
    if(skill2_cnt>=player.skill_2)return;
    skill2_cnt++;
    ui->cnt2->setText(QString::number(player.skill_2-skill2_cnt));
    AudioManager::GetInstance()->PlaySkill();
    board->skyshiv(1);
}

void SingleWindow::on_skill3_button_clicked() {
    if(skill3_cnt>=player.skill_3)return;
    skill3_cnt++;
    ui->cnt3->setText(QString::number(player.skill_3-skill3_cnt));
    AudioManager::GetInstance()->PlaySkill();
    board->generate(0);
}



void SingleWindow::on_pause_button_clicked() {
    timer->stop();
    PauseWindow *pw = new PauseWindow(1, this);
    AudioManager::GetInstance()->PauseBgm2();
    pw->setGeometry(0,0,1280,720);
    pw->show();
    connect(pw, &PauseWindow::exitwindow, this, [this]{
        delete timer;
        AudioManager::GetInstance()->StopBgm2();
        ResultWindow *rw = new ResultWindow(true,
                                                board->getScore(1) + board->getScore(2) + board->getScore(3) + board->getScore(4),
                                                board->getScore1(),
                                                board->getScore2(),
                                                board->getScore3(),
                                                board->getScore4(),
                                                board->getScore5(),
                                                this);
            rw->move(this->pos().x(), this->pos().y());
            rw->show();
            rw->showGem();
            connect(rw, &ResultWindow::exitwindow, this, [=]{
                rw->close();
                changeWindow(new MainWindow());
            });
    });
    connect(pw, &PauseWindow::gameContinue, this, [this]{
        timer->start();
    });
}
