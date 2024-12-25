#include <QInputDialog>
#include <QMessageBox>
#include <QWebSocket>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include "basewindow.h"
#include "resultwindow.h"
#include "ui_resultwindow.h"
#include "mainwindow.h"
#include "../audio/audiomanager.h"
#include "../config/globalconfig.h"
const QPoint show_board_size(60, 60);
const QVector<QPoint> show_opengl_up_left = {QPoint(450, 280), QPoint(450, 420), QPoint(770, 280), QPoint(770, 420)};
//const QUrl serverUrl("ws://localhost:1479");
ResultWindow::ResultWindow(bool isSolo_,int score_, int score1, int score2, int score3, int score4, int score5, QWidget *parent)
    : BaseWindow(parent)
    , ui(new Ui::ResultWindow)
{
    QUrl playerip = BaseWindow::playerIp;
    server = BaseWindow::server;
    isSolo=isSolo_;
    ui->setupUi(this);
    QString part1 = ":/images/singlewindow/background";
    QString picid = QString::number(GlobalConfig::getInstance().getPicStyle() + 1);
    QString root = part1.append(picid)+ ".png";
    ui->resultpic->setStyleSheet(
        QString("#resultpic{border-image:url(%1)}").arg(root)
        );
    score=score_;
    ui->score->setText(QString::number(score_));
    ui->cnt1->setText(QString::number(score1));
    ui->cnt2->setText(QString::number(score2));
    ui->cnt3->setText(QString::number(score3));
    ui->cnt4->setText(QString::number(score4));
    ui->cnt5->setText(QString::number(score5));
    for (int i = 0; i < 4; i++) {
        gem_render_[i] = new Graphics::RenderManager(ui->resultpic);
        gem_render_[i]->setFixedSize(show_board_size.x(), show_board_size.y());
        gem_render_[i]->setGeometry(show_opengl_up_left[i].x() , show_opengl_up_left[i].y(), gem_render_[i]->width(), gem_render_[i]->height());
    }
    auto a=connect(server,&QWebSocket::binaryMessageReceived,this,[&](const QByteArray &message){
        qDebug()<<message;
    });
    connections.push_back(a);
}

void ResultWindow::showGem() {
    for (int i = 0; i < 4; i++) {
        gem_render_[i]->ShowGem(i + 1);
    }
}

void ResultWindow::on_btnReturn_clicked(){
    emit exitwindow(connections);
}

ResultWindow::~ResultWindow()
{
    delete ui;
}

void ResultWindow::on_btnUpdate_clicked()
{
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
void ResultWindow::keyPressEvent(QKeyEvent *e) {
    if(e->key()==Qt::Key_Escape){
        on_btnReturn_clicked();
    }
}
