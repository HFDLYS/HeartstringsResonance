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
    seed[1]=QRandomGenerator::global()->generate();
    seed[2]=QRandomGenerator::global()->generate();
    seed[3]=QRandomGenerator::global()->generate();
    seed[4]=QRandomGenerator::global()->generate();
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

void Room::broadcast(const QByteArray &info,QWebSocket*player){
    qDebug()<<info;
    emit sendMessage(info,player);
}

void Room::run(){
    QThread::sleep(1);
    QJsonObject cmd,parameter;
    cmd["command"]="start";
    QJsonArray seedsArray;
    seedsArray.append(seed[1]);
    seedsArray.append(seed[2]);
    seedsArray.append(seed[3]);
    seedsArray.append(seed[4]);
    parameter["seeds"] = seedsArray;
    
    for (int i = 1; i <= 4; i++) {
        parameter["playerId"]=i;
        cmd["parameter"]=parameter;
        QJsonDocument json(cmd);
        broadcast(json.toJson(),player[i]);
    }
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
            }
        });
    }
}
