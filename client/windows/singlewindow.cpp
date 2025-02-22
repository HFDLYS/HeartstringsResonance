#include "singlewindow.h"
#include "mainwindow.h"
#include "ui_singlewindow.h"
#include "pausewindow.h"
#include "../audio/audiomanager.h"
#include "../config/globalconfig.h"
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
const int MAX_TIME=120;


SingleWindow::SingleWindow(int seed_,int difficulty_,QWidget *parent)
    : BaseWindow(parent), ui(new Ui::SingleWindow), seed(seed_), difficulty(difficulty_) {
    ui->setupUi(this);
    QString part1 = ":/images/singlewindow/background";
    QString picid = QString::number(GlobalConfig::getInstance().getPicStyle() + 1);
    QString root = part1.append(picid)+ ".png";
    ui->picwidget->setStyleSheet(
        QString("#picwidget{border-image:url(%1)}").arg(root)
        );
    is_pause = false;
    ui->score->setText(QString::number(0));
    ui->skill1_button->setEnabled(1);
    ui->skill2_button->setEnabled(1);
    ui->skill3_button->setEnabled(1);
    ui->boom2->setVisible(false);
    ui->boom5->setVisible(false);
    ui->max_score->setText("最高峰: "+QString::number(player.pointSolo));
    if (difficulty == 1) {
        ui->difficulty->setText("普通");
        max_gem_type = 5;
        ui->difficulty->setStyleSheet("font-size:20px;color: Green;font-family:幼圆;text-align: center;");
    } else if (difficulty == 2) {
        ui->difficulty->setText("困难");
        max_gem_type = 6;
        ui->difficulty->setStyleSheet("font-size:20px;color: Yellow;font-family:幼圆;text-align: center;");
        ui->boom2->setVisible(true);
    } else {
        ui->difficulty->setText("极限");
        max_gem_type = 8;
        ui->difficulty->setStyleSheet("font-size:20px;color: Red;font-family:幼圆;text-align: center;");
        ui->boom5->setVisible(true);
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
    auto a=connect(board, &Board::playMatchSound, [this](int type) {
        if (type == 1) {
            AudioManager::GetInstance()->PlayMatch1();
        } else if (type == 2) {
            AudioManager::GetInstance()->PlayMatch2();
        } else if (type == 3) {
            AudioManager::GetInstance()->PlayMatch3();
        }
    });
    connections.push_back(a);
    board->SetGemManager(renderer_->GetGemManager());
    board->initBoard();
}

void SingleWindow::Release1() { ui->skill1_button->setIcon(QIcon(":/images/SingleWindow/1.png")); }
void SingleWindow::Release2() { ui->skill2_button->setIcon(QIcon(":/images/SingleWindow/2.png")); }
void SingleWindow::Release3() { ui->skill3_button->setIcon(QIcon(":/images/SingleWindow/3.png")); }

void SingleWindow::mousePressEvent(QMouseEvent *event) {
    if (!has_started_) return;
    if (is_pause) return;
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

void SingleWindow::startGame() {
    initBoard();
    ui->progressBar->setValue(MAX_TIME);
    timer=new QTimer(this);
    timer->start(1000);
    auto a=QObject::connect(timer, &QTimer::timeout,[&]{
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
                                                difficulty,
                                                this);
            rw->move(this->pos().x(), this->pos().y());
            rw->show();
            rw->showGem();
            this->hide();
            auto a=connect(rw, &ResultWindow::exitwindow, this, [=](QVector<QMetaObject::Connection> cons){
                for(auto con:cons){
                    connections.push_back(con);
                }
                rw->close();
                changeWindow(new MainWindow());
            });
            connections.push_back(a);
        }
    });
    has_started_=true;
    connections.push_back(a);
}

void SingleWindow::on_skill1_button_clicked() {
    if (!has_started_) {
       return;
    }
    if(skill1_cnt>=player.skill_1)return;
    skill1_cnt++;
    ui->cnt1->setText(QString::number(player.skill_1-skill1_cnt));
    AudioManager::GetInstance()->PlaySkill();
    board->hint();
}

void SingleWindow::on_skill2_button_clicked() {
    if (!has_started_) {
       return;
    }
    if(skill2_cnt>=player.skill_2)return;
    skill2_cnt++;
    ui->cnt2->setText(QString::number(player.skill_2-skill2_cnt));
    AudioManager::GetInstance()->PlaySkill();
    board->skyshiv(0);
}

void SingleWindow::on_skill3_button_clicked() {
    if (!has_started_) {
       return;
    }
    if(skill3_cnt>=player.skill_3)return;
    skill3_cnt++;
    ui->cnt3->setText(QString::number(player.skill_3-skill3_cnt));
    AudioManager::GetInstance()->PlaySkill();
    board->generate(0);
}



void SingleWindow::on_pause_button_clicked() {
    if (!has_started_) {
       return;
    }
    is_pause=1;
    timer->stop();
    pw = new PauseWindow(1, this);
    AudioManager::GetInstance()->PauseBgm2();
    pw->setGeometry(0,0,1280,720);
    pw->show();
    auto a=connect(pw, &PauseWindow::exitwindow, this, [this]{
        delete timer;
        AudioManager::GetInstance()->StopBgm2();
        ResultWindow *rw = new ResultWindow(true,
                                            board->getScore(1) + board->getScore(2) + board->getScore(3) + board->getScore(4),
                                            board->getScore1(),
                                            board->getScore2(),
                                            board->getScore3(),
                                            board->getScore4(),
                                            difficulty,
                                            this);
        rw->move(this->pos().x(), this->pos().y());
        rw->show();
        rw->showGem();
        this->hide();
        auto a=connect(rw, &ResultWindow::exitwindow, this, [=](QVector<QMetaObject::Connection> cons){
            for(auto con:cons){
                connections.push_back(con);
            }
            rw->close();
            changeWindow(new MainWindow());
        });
        connections.push_back(a);
    });
    connections.push_back(a);
    a=connect(pw, &PauseWindow::gameContinue, this, [this]{
        is_pause=0;
        timer->start();
    });
    connections.push_back(a);
}
void SingleWindow::keyPressEvent(QKeyEvent *e) {
    if (is_brower_opened) {
        renderer_->keyPressEvent(e);
        return;
    }
    if(!is_pause){
        switch(e->key()){
        case Qt::Key_Escape:
            on_pause_button_clicked();
            break;
        case Qt::Key_W:
            on_skill1_button_clicked();
            break;
        case Qt::Key_A:
            on_skill2_button_clicked();
            break;
        case Qt::Key_S:
            on_pause_button_clicked();
            break;
        case Qt::Key_D:
            on_skill3_button_clicked();
            break;
        case Qt::Key_B:
            is_brower_opened = !is_brower_opened;
            break;
        }
    }else{
        pw->keyPressEvent(e);
    }
}
