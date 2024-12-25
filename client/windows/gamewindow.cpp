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
const QPoint board_size(520, 520);
const QPoint opengl_up_left(10, 100);
const QPoint opengl_down_right = opengl_up_left + QPoint(board_size.x(), board_size.y());
const QPoint sub_board_size(90, 90);
const QPoint sub_opengl_up_left(570, 40);
const QPoint show_opengl_up_left(700, 70);
const QPoint show_board_size(280, 280);
const QVector<QPoint> show_sub_opengl_up_left = {QPoint(700, 5), QPoint(990, 5), QPoint(700, 645), QPoint(990, 645)};
const int sub_board_size_ = 60;
const int border_size = 10;
const int TITLE_HEIGHT = 30;
GameWindow::GameWindow(QWidget *parent)
    : BaseWindow(parent), ui(new Ui::GameWindow) {
    // QString urlstring = "ws://" + ip + ":" + port;
    //QUrl serverUrl(urlstring);
    ui->setupUi(this);
    ui->cnt1->setText(QString::number(player.skill_1));
    ui->cnt2->setText(QString::number(player.skill_2));
    ui->cnt3->setText(QString::number(player.skill_3));
    QUrl playerip = BaseWindow::playerIp;
    server = BaseWindow::server;
    if(server->isValid()){
        QJsonObject cmd,parameter;
        cmd["command"]="multigame";
        parameter["player"]=player.toJson();
        cmd["parameter"]=parameter;
        QJsonDocument json(cmd);
        qDebug()<<server->sendBinaryMessage(json.toJson());
    }
    WaitingWindow *ww = new WaitingWindow(this);
    auto a=connect(ww,&WaitingWindow::closeGame,this,[&]{
        AudioManager::GetInstance()->StopBgm3();
        changeWindow(new MainWindow());
    });
    connections.push_back(a);
    ww->setGeometry(0,0,1280,720);
    ww->show();
    a=connect(server,&QWebSocket::errorOccurred,this,[&](QAbstractSocket::SocketError error){
        if(!this->isVisible()){
            QMessageBox::warning(this,"连接错误","连接服务器失败");
            AudioManager::GetInstance()->StopBgm3();
            changeWindow(new MainWindow());
        }
    });
    connections.push_back(a);
    a=connect(server,&QWebSocket::binaryMessageReceived,this,[&](const QByteArray &message){
        qDebug()<<message;
        QJsonDocument jsonIn = QJsonDocument::fromJson(message);
        QJsonObject cmd = jsonIn.object();
        if(cmd["command"].toString()=="start"){
            cmd = cmd["parameter"].toObject();
            int playerId = cmd["playerId"].toInt();
            QJsonArray seeds = cmd["seeds"].toArray();
            QJsonArray playerstmp = cmd["players"].toArray();
            for(auto player:playerstmp){
                players.push_back(Player(player.toObject()));
            }
            ui->username_1->setText(players[0].username);
            ui->username_2->setText(players[1].username);
            ui->username_3->setText(players[2].username);
            ui->username_4->setText(players[3].username);
            std::vector<int> seedVector;
            for (const QJsonValue &value : seeds) {
                seedVector.push_back(value.toInt());
            }
            player_id_ = playerId;
            main_board_ = new Board(seedVector[playerId-1]);
            main_board_->SetGemManager(main_renderer_->GetGemManager());
            main_board_->initBoard();
            main_chart_->setValues(playerId, 100);
            auto a=connect(main_board_, &Board::playMatchSound, [this](int type) {
                if (type == 1) {
                    AudioManager::GetInstance()->PlayMatch1();
                } else if (type == 2) {
                    AudioManager::GetInstance()->PlayMatch2();
                } else if (type == 3) {
                    AudioManager::GetInstance()->PlayMatch3();
                }
            });
            connections.push_back(a);
            sub_chart_->setValues(playerId, 100);
            for (int i = 1; i <= 4; i++) {
                show_board_[i] = new Board(seedVector[i-1]);
                show_board_[i]->SetGemManager(show_renderer_[i]->GetGemManager());
                show_board_[i]->initBoard();
                show_chart_[i]->setValues(i, 100);
                show_sub_renderer_[i]->ShowGem(i);
                for (int j = 1; j <= 4; j++) {
                    show_sub_chart_[i]->setValues(j, 0);
                }
                show_sub_chart_[i]->setValues(i, 100);
            }
            sub_renderer_->ShowGem(player_id_);
            has_started_ = true;
            emit gameStart();
        }else if(cmd["command"].toString()=="click"){
            //移动对应小棋盘的棋子
            cmd = cmd["parameter"].toObject();
            int x = cmd["x"].toInt();
            int y = cmd["y"].toInt();
            int playerId = cmd["playerId"].toInt();
            show_board_[playerId]->clicked(x, y);
            if (playerId == player_id_) {
                main_board_->clicked(x, y);
            }
        } else if (cmd["command"].toString()=="skill") {
            //使用技能
            cmd = cmd["parameter"].toObject();
            int skillId = cmd["skillId"].toInt();
            int playerId = cmd["playerId"].toInt();
            if (skillId == 1) {
                show_board_[playerId]->hint();
                if (playerId == player_id_) {
                    main_board_->hint();
                }
            } else if (skillId == 2) {
                for (int i = 1; i <= 4; i++) {
                    show_board_[i]->skyshiv(playerId);
                }
                main_board_->skyshiv(playerId);
            } else if (skillId == 3) {
                show_board_[playerId]->generate(0);
                if (playerId == player_id_) {
                    main_board_->generate(0);
                }
            }
        } else if (cmd["command"].toString()=="status") {
            cmd = cmd["parameter"].toObject();
            QJsonArray stances = cmd["stances"].toArray();
            for (int i = 0; i <= 3; i++) {
                QJsonArray stance = stances[i].toArray();
                for (int j = 0; j <= 3; j++) {
                    show_chart_[i+1]->setValues(j+1, stance[j].toInt());
                    if (i+1 == player_id_) {
                        main_chart_->setValues(j+1, stance[j].toInt());
                        sub_chart_->setValues(j+1, stance[j].toInt());
                    }
                }
                int ret = 0;
                int ans = 0;
                for (int j = 1; j <= 4; j++) {
                    if (stance[j-1].toInt() > ret) {
                        ret = stance[j-1].toInt();
                        ans = j;
                    }
                }
                show_sub_renderer_[i+1]->ShowGem(ans);
                for (int j = 1; j <= 4; j++) {
                    show_sub_chart_[i+1]->setValues(j, 0);
                }
                show_sub_chart_[i+1]->setValues(ans, 100);
                if (i+1 == player_id_) {
                    sub_renderer_->ShowGem(ans);
                    for (int j = 1; j <= 4; j++) {
                        sub_chart_->setValues(j, 0);
                    }
                    sub_chart_->setValues(ans, 100);
                }
            }
        } else if(cmd["command"].toString()=="end"){
            //结束游戏(这里是直接复制单人模式的)
            AudioManager::GetInstance()->StopBgm2();
            ResultWindow *rw = new ResultWindow(false,1,1,4,5,1,4,this);
            rw->move(this->pos().x(), this->pos().y());
            rw->show();
            auto a=connect(rw, &ResultWindow::exitwindow, this, [=](QVector<QMetaObject::Connection> cons){
                for(auto con:cons){
                    connections.push_back(con);
                }
                rw->close();
                changeWindow(new MainWindow());
            });
        }
    });
    connections.push_back(a);
    main_chart_ = new SquarePieChart(ui->controlWidget);
    main_chart_->setGeometry(opengl_up_left.x()-3, opengl_up_left.y()-3, board_size.x()+6, board_size.y()+6);
    sub_chart_ = new SquarePieChart(ui->controlWidget);
    sub_chart_->setGeometry(sub_opengl_up_left.x()-2, sub_opengl_up_left.y()-2, sub_board_size.x()+4, sub_board_size.y()+4);
    sub_renderer_ = new Graphics::RenderManager(ui->controlWidget);
    main_renderer_ = new Graphics::RenderManager(ui->controlWidget);
    sub_renderer_->setFixedSize(sub_board_size.x(), sub_board_size.y());
    sub_renderer_->setGeometry(sub_opengl_up_left.x(), sub_opengl_up_left.y(), sub_renderer_->width(), sub_renderer_->height());
    main_renderer_->setFixedSize(board_size.x(), board_size.y());
    main_renderer_->setGeometry(opengl_up_left.x(), opengl_up_left.y(), main_renderer_->width(), main_renderer_->height());
    for (int i = 1; i <= 4; i++) {
        int ix = (i-1) % 2;
        int iy = (i-1) / 2;
        show_chart_[i] = new SquarePieChart(ui->controlWidget);
        show_chart_[i]->setGeometry(show_opengl_up_left.x()-3 + ix * (show_board_size.x() + border_size), show_opengl_up_left.y()-3 + iy * (show_board_size.y() + border_size), show_board_size.x()+6, show_board_size.y()+6);
        show_sub_chart_[i] = new SquarePieChart(ui->controlWidget);
        show_sub_chart_[i]->setGeometry(show_sub_opengl_up_left[i-1].x()-1, show_sub_opengl_up_left[i-1].y()-1, sub_board_size_+2, sub_board_size_+2);
        show_renderer_[i] = new Graphics::RenderManager(ui->controlWidget);
        show_renderer_[i]->setFixedSize(show_board_size.x(), show_board_size.y());
        show_renderer_[i]->setGeometry(show_opengl_up_left.x() + ix * (show_board_size.x() + border_size), show_opengl_up_left.y() + iy * (show_board_size.y() + border_size), show_renderer_[i]->width(), show_renderer_[i]->height());
        show_sub_renderer_[i] = new Graphics::RenderManager(ui->controlWidget);
        show_sub_renderer_[i]->setFixedSize(sub_board_size_, sub_board_size_);
        show_sub_renderer_[i]->setGeometry(show_sub_opengl_up_left[i-1].x(), show_sub_opengl_up_left[i-1].y(), show_sub_renderer_[i]->width(), show_sub_renderer_[i]->height());

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
    if (event->y() < TITLE_HEIGHT) {
        last = event->globalPosition().toPoint();
    }
    if (!has_started_) return;
    // board->Clicked(x, y);
    if (x > opengl_up_left.x() && y > opengl_up_left.y() && x < opengl_down_right.x() && y < opengl_down_right.y()) {

        x -= opengl_up_left.x();
        y -= opengl_up_left.y();
        std::cout << "mouse cliked on:" << x << " " << y << std::endl;
        std::cout <<  8 * x /(board_size.x())*8 + 8 * y/(board_size.y())+1 << '\n';
        int nx = 8 * x/(board_size.x());
        int ny = 8 * y/(board_size.y());
        int nd = x /(board_size.x() / 8)*8 + y/(board_size.y() / 8)+1;
        // main_renderer_->GetGemManager()->Remove(nd, true);
        // std::cout << nd << '\n';
        QJsonObject cmd,parameter;
        cmd["command"]="click";
        parameter["x"]=nx;
        parameter["y"]=ny;
        cmd["parameter"]=parameter;
        QJsonDocument json(cmd);
        server->sendBinaryMessage(json.toJson());
        // main_board_->clicked(nx, ny);
    }
}
void GameWindow::startGame() {

        // main_renderer_->Demo();
}

void GameWindow::on_skill1_button_clicked() {
    if(skill1_cnt++>=player.skill_1)return;
    ui->cnt1->setText(QString::number(player.skill_1-skill1_cnt));
    AudioManager::GetInstance()->PlaySkill();
    QJsonObject cmd,parameter;
    cmd["command"]="skill";
    parameter["skillId"]=1;
    cmd["parameter"]=parameter;
    QJsonDocument json(cmd);
    server->sendBinaryMessage(json.toJson());
    // main_board_->hint();
}

void GameWindow::on_skill2_button_clicked() {
    if(skill2_cnt++>=player.skill_2)return;
    ui->cnt2->setText(QString::number(player.skill_2-skill2_cnt));
    AudioManager::GetInstance()->PlaySkill();
    QJsonObject cmd,parameter;
    cmd["command"]="skill";
    parameter["skillId"]=2;
    cmd["parameter"]=parameter;
    QJsonDocument json(cmd);
    server->sendBinaryMessage(json.toJson());
    // main_board_->skyshiv(player_id_);
}

void GameWindow::on_skill3_button_clicked() {
    if(skill3_cnt++>=player.skill_3)return;
    ui->cnt3->setText(QString::number(player.skill_3-skill3_cnt));
    AudioManager::GetInstance()->PlaySkill();
    QJsonObject cmd,parameter;
    cmd["command"]="skill";
    parameter["skillId"]=3;
    cmd["parameter"]=parameter;
    QJsonDocument json(cmd);
    server->sendBinaryMessage(json.toJson());
    // main_board_->generate(0);
}

void GameWindow::on_pause_button_clicked() {
    AudioManager::GetInstance()->PauseBgm3();
    PauseWindow *pw = new PauseWindow(2, this);
    pw->setGeometry(0,0,1280,720);
    pw->show();
    AudioManager::GetInstance()->PauseBgm2();
    auto a=connect(pw, &PauseWindow::exitwindow, this, [this]{
        AudioManager::GetInstance()->StopBgm3();
        changeWindow(new MainWindow());
    });
    connections.push_back(a);
}
