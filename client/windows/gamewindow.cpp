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
QPoint toPoint(int, int);
const QPoint board_size_main(520, 520);
const QPoint board_size_sub(280, 280);
const QPoint opengl_up_left[10] = {toPoint(10, 100), toPoint(700, 70), toPoint(990, 70), toPoint(700,360), toPoint(990,360)};
const QPoint opengl_down_right[10] = {toPoint(530,620),toPoint(980, 350),toPoint(1270,350),toPoint(980,640),toPoint(1270,640)};
const QPoint show_opengl_up_left(700, 70);
const QPoint show_board_size(280, 280);
const int border_size = 10;
const int TITLE_HEIGHT = 30;
const int SERVER_PORT=1479;
const int seed = 0; // tmperary
const QUrl serverUrl("ws://localhost:1479");
QPoint toPoint(int x, int y){
    QPoint point(x,y);
    return point;
}
GameWindow::GameWindow(QWidget *parent)
    : BaseWindow(parent), ui(new Ui::GameWindow) {
    ui->setupUi(this);
    for(int i = 0; i <= 4; i++){
        renderer_[i] = new Graphics::RenderManager(ui->controlWidget);
        if(i == 0) renderer_[i]->setFixedSize(board_size_main.x(), board_size_main.y());
        else renderer_[i]->setFixedSize(board_size_sub.x(), board_size_sub.y());
        renderer_[i]->setGeometry(opengl_up_left[i].x(), opengl_up_left[i].y(), renderer_[i]->width(), renderer_[i]->height());
    }
    // server=new QWebSocket();
    // server->open(serverUrl);
    // WaitingWindow *ww = new WaitingWindow(this);
    // ww->setGeometry(0,0,1280,720);
    // ww->show();
    // connect(server,&QWebSocket::binaryMessageReceived,this,[&](const QByteArray &message){
    //     qDebug()<<message;
    //     QJsonDocument jsonIn=QJsonDocument::fromJson(message);
    //     QJsonObject cmd=jsonIn.object();
    //     if(cmd["command"].toString()=="start"){
    //         qDebug()<<cmd["seed"];
    //         seed=cmd["seed"].toInt();
    //         emit gameStart();//尽量不要改可能因不明原因崩溃.
    //         startGame();
    //     }else if(cmd["command"].toString()=="click"){
    //         //移动对应小棋盘的棋子

    //     }else if(cmd["command"].toString()=="end"){
    //         //结束游戏(这里是直接复制单人模式的)
    //         AudioManager::GetInstance()->StopBgm2();
    //         ResultWindow *rw = new ResultWindow(false,1,1,4,5,1,4,this);
    //         rw->move(this->pos().x(), this->pos().y());
    //         rw->show();
    //         connect(rw, &ResultWindow::exitwindow, this, [=]{
    //             rw->close();
    //             changeWindow(new MainWindow());
    //         });
    //     }
    // });
    main_renderer_ = new Graphics::RenderManager(ui->controlWidget);
    main_renderer_->setFixedSize(board_size_main.x(), board_size_main.y());
    main_renderer_->setGeometry(opengl_up_left[0].x(), opengl_up_left[0].y(), main_renderer_->width(), main_renderer_->height());
    
    for (int i = 0; i < 4; i++) {
        sub_renderer_[i] = new Graphics::RenderManager(ui->controlWidget);
        sub_renderer_[i]->setFixedSize(show_board_size.x(), show_board_size.y());
        int ix = i % 2;
        int iy = i / 2;
        sub_renderer_[i]->setGeometry(show_opengl_up_left.x() + ix * (show_board_size.x() + border_size), show_opengl_up_left.y() + iy * (show_board_size.y() + border_size), sub_renderer_[i]->width(), sub_renderer_[i]->height());
    }
}

GameWindow::~GameWindow() {

}
void GameWindow::Release1() { ui->skill1_button->setIcon(QIcon(":/images/gamewindow/1.png")); }
void GameWindow::Release2() { ui->skill2_button->setIcon(QIcon(":/images/gamewindow/2.png")); }
void GameWindow::Release3() { ui->skill3_button->setIcon(QIcon(":/images/gamewindow/3.png")); }

void GameWindow::mousePressEvent(QMouseEvent *event) {
    int x = event->position().x();
    int y = event->position().y();
    // std::cout << "mouse cliked on:" << x << " " << y << std::endl;
    if (event->position().y() < TITLE_HEIGHT) {
        last = event->globalPosition().toPoint();
    }
    // board->Clicked(x, y);
    // if (x > opengl_up_left.x() && y > opengl_up_left.y() && x < opengl_down_right.x() && y < opengl_down_right.y()) {
        
    //     x -= opengl_up_left.x();
    //     y -= opengl_up_left.y();
    //     std::cout << "mouse cliked on:" << x << " " << y << std::endl;
    //     std::cout <<  8 * x /(board_size.x())*8 + 8 * y/(board_size.y())+1 << '\n';
    //     int nx = 8 * x/(board_size.x());
    //     int ny = 8 * y/(board_size.y());
    //     int nd = x /(board_size.x() / 8)*8 + y/(board_size.y() / 8)+1;
    //     main_renderer_->GetGemManager()->Remove(nd, true);
    //     // std::cout << nd << '\n';
    // }
}
void GameWindow::startGame() {

    initBoard();
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

void GameWindow::initBoard(){
    for(int i = 0; i <= 4; i++){
        board[i] = new Board(seed);
        board[i]->SetGemManager(renderer_[i]->GetGemManager());
        board[i]->initBoard();
    }

}
