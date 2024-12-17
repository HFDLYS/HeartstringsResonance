#include "gamewindow.h"
#include "mainwindow.h"
#include "ui_gamewindow.h"
#include "pausewindow.h"
#include "waitingwindow.h"
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
#include <random>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>

/*
 * 未实现功能:
 * 2.交换宝石时发送信息至服务器.(格式:click/x1/y1/x2/y2)
 * 3.收到服务器交换棋子的信息(格式:click/x1/y1/x2/y2/playerId)时交换全局版面的对应宝石.
 * 4.开始游戏时将服务器返回的种子填入游戏.
*/

const QPoint board_size(500, 500);
const QPoint opengl_up_left(25, 100);
const QPoint opengl_down_right = opengl_up_left + QPoint(board_size.x(), board_size.y());
const int TITLE_HEIGHT = 30;
const int SERVER_PORT=1479;
const QUrl serverUrl("ws://localhost:1479");
GameWindow::GameWindow(QWidget *parent)
    : BaseWindow(parent), ui(new Ui::GameWindow) {
    ui->setupUi(this);
    server=new QWebSocket();
    server->open(serverUrl);
    WaitingWindow *ww = new WaitingWindow(this);
    ww->setGeometry(0,0,1280,720);
    ww->show();
    connect(server,&QWebSocket::binaryMessageReceived,this,[&](const QByteArray &message){
        qDebug()<<message;
        QJsonDocument jsonIn=QJsonDocument::fromJson(message);
        QJsonObject cmd=jsonIn.object();
        if(cmd["command"].toString()=="start"){
            qDebug()<<cmd["seed"];
            seed=cmd["seed"].toInt();
            emit gameStart();//尽量不要改可能因不明原因崩溃.
            startGame();
        }else if(cmd["command"].toString()=="click"){
            //移动对应小棋盘的棋子

        }else if(cmd["command"].toString()=="end"){
            //结束游戏(这里是直接复制单人模式的)
            AudioManager::GetInstance()->StopBgm2();
            ResultWindow *rw = new ResultWindow(114514,this);
            rw->setGeometry(0,0,1280,720);
            rw->show();
            connect(rw, &ResultWindow::exitwindow, this, [this]{
                changeWindow(new MainWindow());
            });
        }
    });
    renderer_ = new Graphics::RenderManager(ui->controlWidget);
    renderer_->setFixedSize(board_size.x(), board_size.y());
    renderer_->setGeometry(opengl_up_left.x(), opengl_up_left.y(), renderer_->width(), renderer_->height());

}

GameWindow::~GameWindow() {

}
void GameWindow::Release1() { ui->skill1_button->setIcon(QIcon(":/images/gamewindow/1.png")); }
void GameWindow::Release2() { ui->skill2_button->setIcon(QIcon(":/images/gamewindow/2.png")); }
void GameWindow::Release3() { ui->skill3_button->setIcon(QIcon(":/images/gamewindow/3.png")); }

void GameWindow::mousePressEvent(QMouseEvent *event) {
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
        std::cout <<  8 * x /(board_size.x())*8 + 8 * y/(board_size.y())+1 << '\n';
        int nx = 8 * x/(board_size.x());
        int ny = 8 * y/(board_size.y());
        int nd = x /(board_size.x() / 8)*8 + y/(board_size.y() / 8)+1;
        renderer_->GetGemManager()->Remove(nd, true);
        // std::cout << nd << '\n';
    }
}
void GameWindow::startGame() {

    renderer_->Demo();
}

void GameWindow::on_skill1_button_clicked() {
    AudioManager::GetInstance()->PlaySkill();

}

void GameWindow::on_skill2_button_clicked() {
    AudioManager::GetInstance()->PlaySkill();

}

void GameWindow::on_skill3_button_clicked() {
    AudioManager::GetInstance()->PlaySkill();

}

void GameWindow::on_pause_button_clicked() {
    AudioManager::GetInstance()->PauseBgm3();
    PauseWindow *pw = new PauseWindow(2, this);
    pw->setGeometry(0,0,1280,720);
    pw->show();
    AudioManager::GetInstance()->PauseBgm2();
    connect(pw, &PauseWindow::exitwindow, this, [this]{
        AudioManager::GetInstance()->StopBgm3();
        changeWindow(new MainWindow());
    });
}
