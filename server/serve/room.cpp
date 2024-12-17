#include <QRandomGenerator>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include "room.h"
/*
 * 未完成:
 * 1.倒计时结束游戏.
*/
Room::Room(int id_,QWebSocket*w,QWebSocket*a,QWebSocket*s,QWebSocket*d,QObject *parent):QThread(parent){
    id=id_;
    player[1]=w;
    player[2]=a;
    player[3]=s;
    player[4]=d;
    seed=QRandomGenerator::global()->generate();
}
int Room::getId(){
    return id;
}
void Room::broadcast(const QByteArray &info){
    qDebug()<<info;
    for(int i=1;i<=4;i++){
        emit sendMessage(info,player[i]);
    }
}
void Room::run(){
    QThread::sleep(3);
    QJsonObject cmd,parameter;
    cmd["command"]="start";
    parameter["seed"]=seed;
    cmd["parameter"]=parameter;
    QJsonDocument json(cmd);
    broadcast(json.toJson());
    for(int i=1;i<=4;i++){
        connect(player[i],&QWebSocket::binaryMessageReceived,this,[=](const QByteArray &message){
            QJsonDocument jsonIn=QJsonDocument::fromJson(message);
            QJsonObject cmd=jsonIn.object();
            if(cmd["command"].toString()=="click"){
                QJsonObject parameter=cmd["parameter"].toObject();
                parameter["player"]=i;
                cmd["parameter"]=parameter;
                QJsonDocument jsonOut(cmd);
                broadcast(jsonOut.toJson());
            }else if(cmd["command"].toString()=="updatePoint"){
                QJsonObject parameter=cmd["parameter"].toObject();
                emit updatePoint(parameter["userName"].toString(),parameter["point"].toInt());
            }
        });
    }
}
