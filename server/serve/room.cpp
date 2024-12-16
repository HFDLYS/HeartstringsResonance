#include <QRandomGenerator>

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
void Room::broadcast(QString info){
    qDebug()<<info;
    for(int i=1;i<=4;i++){
        emit sendMessage(info,player[i]);
    }
}
void Room::run(){
    QThread::sleep(3);
    broadcast(QString("start/%1").arg(seed));
    for(int i=1;i<=4;i++){
        connect(player[i],&QWebSocket::textMessageReceived,this,[=](QString message){
            QStringList info=message.split("/");
            if(info[0]=="click"){
                broadcast(message+"/"+QString::number(i));
            }
        });
        //发送结束指令
    }
}
