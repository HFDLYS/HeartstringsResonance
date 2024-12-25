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
//const QUrl serverUrl("ws://localhost:1479");
MultiResultWindow::MultiResultWindow(QWidget *parent)
    : BaseWindow(parent)
    , ui(new Ui::MultiResultWindow)
{
    QUrl playerip = BaseWindow::playerIp;
    server = BaseWindow::server;
    for (int i = 0; i < 4; i++) {
        gem_render_[i] = new Graphics::RenderManager(ui->resultpic);
        gem_render_[i]->setFixedSize(show_board_size.x(), show_board_size.y());
        gem_render_[i]->setGeometry(show_opengl_up_left.x() + i * (BORDER_SIZE), show_opengl_up_left.y(), gem_render_[i]->width(), gem_render_[i]->height());
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

void MultiResultWindow::on_btnUpdate_clicked()
{
    return;
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
    parameter["pointSolo"]=isSolo?score:0;
    parameter["pointMulti"]=isSolo?0:score;
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

