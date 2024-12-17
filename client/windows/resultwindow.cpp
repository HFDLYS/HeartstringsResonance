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

const QPoint show_opengl_up_left(500, 280);
const QPoint show_board_size(40, 40);
const QUrl serverUrl("ws://localhost:1478");
ResultWindow::ResultWindow(bool isSolo_,int score_, int score1, int score2, int score3, int score4, int score5, QWidget *parent)
    : BaseWindow(parent)
    , ui(new Ui::ResultWindow)
{
    server=new QWebSocket();
    server->open(serverUrl);
    isSolo=isSolo_;
    ui->setupUi(this);
    score=score_;
    ui->score->setText(QString::number(score_));
    ui->cnt1->setText(QString::number(score1));
    ui->cnt2->setText(QString::number(score2));
    ui->cnt3->setText(QString::number(score3));
    ui->cnt4->setText(QString::number(score4));
    ui->cnt5->setText(QString::number(score5));
    for (int i = 0; i < 5; i++) {
        gem_render_[i] = new Graphics::RenderManager(ui->resultpic);
        gem_render_[i]->setFixedSize(show_board_size.x(), show_board_size.y());
        gem_render_[i]->setGeometry(show_opengl_up_left.x() , show_opengl_up_left.y() + i * (show_board_size.y()+10), gem_render_[i]->width(), gem_render_[i]->height());
    }
    connect(server,&QWebSocket::binaryMessageReceived,this,[&](const QByteArray &message){
        qDebug()<<message;
    });
}

void ResultWindow::showGem() {
    for (int i = 0; i < 5; i++) {
        gem_render_[i]->ShowGem(i + 1);
    }
}

void ResultWindow::on_btnReturn_clicked(){
    emit exitwindow();
}

ResultWindow::~ResultWindow()
{
    delete ui;
}

void ResultWindow::on_btnUpdate_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this,
                                         "上传成绩",
                                         "请写下您的大名:",
                                         QLineEdit::Normal,
                                         "",
                                         &ok);

    if (ok && !text.isEmpty()) {
        //在这里补充数据库逻辑
        QJsonObject cmd,parameter;
        cmd["command"]="updatePoint";
        parameter["userName"]=text;
        parameter["pointSolo"]=isSolo?score:0;
        parameter["pointMulti"]=isSolo?0:score;
        cmd["parameter"]=parameter;
        QJsonDocument json(cmd);
        qDebug()<<server->sendBinaryMessage(json.toJson());
        QMessageBox::information(this, "上传成功", text+"的成绩已上传");
        emit exitwindow();
    } else {
        QMessageBox::information(this, "取消", "我的名字呢？");
        emit exitwindow();
    }
}

