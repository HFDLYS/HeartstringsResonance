#include <QRandomGenerator>

#include "room.h"

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
    emit broadcastSignal(info);
    for(int i=1;i<=4;i++){
        if(player[i]->isValid()){
            player[i]->sendTextMessage(info);
        }
    }
}
void Room::run(){
    broadcast(QString("start/%1").arg(seed));
    for(int i=1;i<=4;i++){
        connect(player[i],&QWebSocket::textMessageReceived,this,[=](QString message){
            QStringList info=message.split("/");
            if(info[0]=="click"){
                broadcast(message+"/"+QString::number(i));
            }
        });
    }
}
