#include "rankwindow.h"
#include <QFont>
#include <QFontDatabase>
#include <QWebSocket>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include "mainwindow.h"
#include "ui_rankwindow.h"
#include "../audio/audiomanager.h"
#include "../config/globalconfig.h"
RankWindow::RankWindow(QWidget *parent) : BaseWindow(parent), ui(new Ui::RankWindow) {
    ui->setupUi(this);
    // 固定窗口大小

    QString part1 = ":/images/singlewindow/background";
    QString picid = QString::number(GlobalConfig::getInstance().getPicStyle() + 1);
    QString root = part1.append(picid)+ ".png";
    ui->rankpic->setStyleSheet(
        QString("#rankpic{border-image:url(%1)}").arg(root)
        );
    this->setFixedSize(1280, 720);
    // 去除自带的边框
    this->setWindowFlag(Qt::FramelessWindowHint);
    QUrl playerip = BaseWindow::playerIp;
    server = BaseWindow::server;
    QJsonObject cmd,parameter;
    cmd["command"]="rank";
    parameter["length"] = 10;
    cmd["parameter"]=parameter;
    QJsonDocument json(cmd);
    qDebug() << server->sendBinaryMessage(json.toJson());

    auto a=connect(server,&QWebSocket::binaryMessageReceived,this,[&](const QByteArray &message){
        qDebug()<<"\n\n\nrank\n\n\n"<<message;
        QJsonDocument jsonIn = QJsonDocument::fromJson(message);
        QJsonObject cmd = jsonIn.object();
        if(cmd["command"].toString()=="rank"){
            QJsonObject parameter;
            parameter = cmd["parameter"].toObject();
            QJsonArray soloArray = parameter["soloRank"].toArray();
            QJsonArray multiArray = parameter["multiRank"].toArray();
            int i = 0;
            for(auto soloPlayer : soloArray){
                QString id = soloPlayer.toObject()["userName"].toString();
                int score = soloPlayer.toObject()["point"].toInt();
                id_solo[++i] = id;
                score_solo[i] = score;
                qDebug() << id << ' ' << score << '\n';
            }
            i = 0;
            for(auto multiPlayer : multiArray){
                QString id = multiPlayer.toObject()["userName"].toString();
                int score = multiPlayer.toObject()["point"].toInt();
                id_multi[++i] = id;
                score_multi[i] = score;
                qDebug() << id << ' ' << score << '\n';
            }
        }
        ui->id_1->raise();
        ui->id_1->setText(id_solo[1]); ui->score_1->setText(QString::number(score_solo[1]));
        ui->id_2->setText(id_solo[2]); ui->score_2->setText(QString::number(score_solo[2]));
        ui->id_3->setText(id_solo[3]); ui->score_3->setText(QString::number(score_solo[3]));
        ui->id_4->setText(id_solo[4]); ui->score_4->setText(QString::number(score_solo[4]));
        ui->id_5->setText(id_solo[5]); ui->score_5->setText(QString::number(score_solo[5]));
        ui->id_6->setText(id_solo[6]); ui->score_6->setText(QString::number(score_solo[6]));
        ui->id_7->setText(id_solo[7]); ui->score_7->setText(QString::number(score_solo[7]));
        ui->id_8->setText(id_solo[8]); ui->score_8->setText(QString::number(score_solo[8]));
        ui->id_9->setText(id_solo[9]); ui->score_9->setText(QString::number(score_solo[9]));
        ui->id_10->setText(id_solo[10]); ui->score_10->setText(QString::number(score_solo[10]));
        ui->id_1->raise(); ui->score_1->raise();
        ui->id_2->raise(); ui->score_2->raise();
        ui->id_3->raise(); ui->score_3->raise();
        ui->id_4->raise(); ui->score_4->raise();
        ui->id_5->raise(); ui->score_5->raise();
        ui->id_6->raise(); ui->score_6->raise();
        ui->id_7->raise(); ui->score_7->raise();
        ui->id_8->raise(); ui->score_8->raise();
        ui->id_9->raise(); ui->score_9->raise();
        ui->id_10->raise(); ui->score_10->raise();


        ui->id_11->setText(id_multi[1]); ui->score_11->setText(QString::number(score_multi[1]));
        ui->id_12->setText(id_multi[2]); ui->score_12->setText(QString::number(score_multi[2]));
        ui->id_13->setText(id_multi[3]); ui->score_13->setText(QString::number(score_multi[3]));
        ui->id_14->setText(id_multi[4]); ui->score_14->setText(QString::number(score_multi[4]));
        ui->id_15->setText(id_multi[5]); ui->score_15->setText(QString::number(score_multi[5]));
        ui->id_16->setText(id_multi[6]); ui->score_16->setText(QString::number(score_multi[6]));
        ui->id_17->setText(id_multi[7]); ui->score_17->setText(QString::number(score_multi[7]));
        ui->id_18->setText(id_multi[8]); ui->score_18->setText(QString::number(score_multi[8]));
        ui->id_19->setText(id_multi[9]); ui->score_19->setText(QString::number(score_multi[9]));
        ui->id_20->setText(id_multi[10]); ui->score_20->setText(QString::number(score_multi[10]));

    });
    connections.push_back(a);
}
void RankWindow::keyPressEvent(QKeyEvent *e) {
    if(e->key()==Qt::Key_Escape){
        on_btnReturn_clicked();
    }
}
RankWindow::~RankWindow() { delete ui; }

void RankWindow::on_btnReturn_clicked() {
    AudioManager::GetInstance()->PlayLabel();
    changeWindow(new MainWindow());
}
