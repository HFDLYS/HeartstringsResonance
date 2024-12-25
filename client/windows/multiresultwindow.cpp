#include <QInputDialog>
#include <QMessageBox>
#include <QWebSocket>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include "basewindow.h"
#include "multiresultwindow.h"
#include "ui_multiresultwindow.h"
#include "mainwindow.h"
#include "../audio/audiomanager.h"

const QPoint show_board_size(60, 60);
const QPoint show_opengl_up_left(290, 150);
const int BORDER_SIZE = 100;
const QPoint my_show_opengl_up_left[4] = {QPoint(900, 240), QPoint(1030, 240), QPoint(900, 380), QPoint(1030, 380)};
const int INIT_STANCE = 100;
//const QUrl serverUrl("ws://localhost:1479");
MultiResultWindow::MultiResultWindow(QJsonObject parameter,int playerId, QWidget *parent)
    : BaseWindow(parent)
    , ui(new Ui::MultiResultWindow)
{
    ui->setupUi(this);
    QUrl playerip = BaseWindow::playerIp;
    server = BaseWindow::server;
    for (int i = 0; i < 4; i++) {
        gem_render_[i] = new Graphics::RenderManager(ui->resultpic);
        gem_render_[i]->setFixedSize(show_board_size.x(), show_board_size.y());
        gem_render_[i]->setGeometry(show_opengl_up_left.x(), show_opengl_up_left.y() + i * (BORDER_SIZE), gem_render_[i]->width(), gem_render_[i]->height());
    }
    for (int i = 0; i < 4; i++) {
        my_gem_render_[i] = new Graphics::RenderManager(ui->resultpic);
        my_gem_render_[i]->setFixedSize(show_board_size.x(), show_board_size.y());
        my_gem_render_[i]->setGeometry(my_show_opengl_up_left[i].x(), my_show_opengl_up_left[i].y(), my_gem_render_[i]->width(), my_gem_render_[i]->height());
    }
    auto a=connect(server,&QWebSocket::binaryMessageReceived,this,[&](const QByteArray &message){
        qDebug()<<message;
    });
    connections.push_back(a);
    qDebug()<<parameter;
    QJsonArray playerstmp = parameter["players"].toArray();
    QVector<Player> players;
    for(auto player:playerstmp){
        players.push_back(Player(player.toObject()));
    }
    ui->username_1->setText(players[0].username);
    ui->username_2->setText(players[1].username);
    ui->username_3->setText(players[2].username);
    ui->username_4->setText(players[3].username);
    QJsonArray stances = parameter["stances"].toArray();
    int player[5] = {0, -INIT_STANCE, -INIT_STANCE, -INIT_STANCE, -INIT_STANCE};
    int my_data[5] = {0, 0, 0, 0, 0};
    for (int i = 0; i < 4; i++) {
        QJsonArray stance = stances[i].toArray();
        for (int j = 0; j < 4; j++) {
            player[j + 1] += stance[j].toInt();
            if (j + 1 == playerId) {
                my_data[i + 1] = stance[j].toInt();
                if (i + 1 == playerId) {
                    my_data[i + 1] -= INIT_STANCE;
                }
            }
        }
    }
    score=my_data[1] + my_data[2] + my_data[3] + my_data[4];
    ui->score->setText(QString::number(my_data[1] + my_data[2] + my_data[3] + my_data[4]));
    ui->cnt1->setText(QString::number(player[1]));
    ui->cnt2->setText(QString::number(player[2]));
    ui->cnt3->setText(QString::number(player[3]));
    ui->cnt4->setText(QString::number(player[4]));
    ui->my_cnt_1->setText(QString::number(my_data[1]));
    ui->my_cnt_2->setText(QString::number(my_data[2]));
    ui->my_cnt_3->setText(QString::number(my_data[3]));
    ui->my_cnt_4->setText(QString::number(my_data[4]));
}

void MultiResultWindow::showGem() {
    for (int i = 0; i < 4; i++) {
        gem_render_[i]->ShowGem(i + 1);
    }
    for (int i = 0; i < 4; i++) {
        my_gem_render_[i]->ShowGem(i + 1);
    }
}

void MultiResultWindow::on_btnReturn_clicked(){
    emit exitwindow(connections);
}

MultiResultWindow::~MultiResultWindow()
{
    delete ui;
}
void MultiResultWindow::keyPressEvent(QKeyEvent *e) {
    if(e->key()==Qt::Key_Escape){
        on_btnReturn_clicked();
    }
}
void MultiResultWindow::on_btnUpdate_clicked()
{   
    // todo: implement this function
    //return;
    bool ok;
    /*QString text = QInputDialog::getText(this,
                                         "上传成绩",
                                         "请写下您的大名:",
                                         QLineEdit::Normal,
                                         "",
                                         &ok);*/
    //if (ok && !text.isEmpty()) {
    //在这里补充数据库逻辑
    QJsonObject cmd,parameter;
    cmd["command"]="updatePoint";
    parameter["userName"]=player.username;
    parameter["pointSolo"]=0;
    parameter["pointMulti"]=score;
    cmd["parameter"]=parameter;
    QJsonDocument json(cmd);
    qDebug()<<server->sendBinaryMessage(json.toJson());
    auto a=connect(server,&QWebSocket::binaryMessageReceived,this,[&](const QByteArray &message){
        QJsonDocument jsonIn = QJsonDocument::fromJson(message);
        QJsonObject cmd = jsonIn.object();
        // qDebug() << cmd;
        QJsonObject parameter;
        parameter = cmd["parameter"].toObject();
        setPlayer(Player(parameter["player"].toObject()));
        QMessageBox::information(this, "上传成功", player.username+"的成绩已上传");
        emit exitwindow(connections);
    });
    connections.push_back(a);
}

