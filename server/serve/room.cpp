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
    gem_manager = new ServerGemManager();
    seed[1]=QRandomGenerator::global()->generate();
    seed[2]=QRandomGenerator::global()->generate();
    seed[3]=QRandomGenerator::global()->generate();
    seed[4]=QRandomGenerator::global()->generate();
    board[1]=new Board(seed[1]);
    board[2]=new Board(seed[2]);
    board[3]=new Board(seed[3]);
    board[4]=new Board(seed[4]);
    board[1]->SetGemManager(gem_manager);
    board[2]->SetGemManager(gem_manager);
    board[3]->SetGemManager(gem_manager);
    board[4]->SetGemManager(gem_manager);
    stance[1][1]=200;
    stance[2][2]=200;
    stance[3][3]=200;
    stance[4][4]=200;
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

int Room::getStance(int ind){
    int ret = 0;
    int ans = 0;
    for (int i = 1; i <= 4; i++) {
        if (stance[ind][i] > ans) {
            ans = stance[ind][i];
            ret = i;
        }
    }
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
            int pre_score[5][5];
            for (int i = 1; i <= 4; i++) {
                for (int j = 1; j <= 4; j++) {
                    pre_score[i][j] = board[i]->getScore(j);
                }
            }
            if(cmd["command"].toString()=="click"){
                QJsonObject parameter=cmd["parameter"].toObject();
                parameter["playerId"]=i;
                cmd["parameter"]=parameter;
                QJsonDocument jsonOut(cmd);
                broadcast(jsonOut.toJson());
                int x=parameter["x"].toInt();
                int y=parameter["y"].toInt();
                int playerId=parameter["playerId"].toInt();
                board[playerId]->clicked(x,y);
            } else if (cmd["command"].toString()=="skill") {
                QJsonObject parameter=cmd["parameter"].toObject();
                parameter["playerId"]=i;
                cmd["parameter"]=parameter;
                QJsonDocument jsonOut(cmd);
                broadcast(jsonOut.toJson());
                int skillId = parameter["skillId"].toInt();
                int playerId = parameter["playerId"].toInt();
                if (skillId == 1) {
                    board[playerId]->hint();
                } else if (skillId == 2) {
                    for (int i = 1; i <= 4; i++) {
                        board[i]->skyshiv(playerId);
                    }
                } else if (skillId == 3) {
                    board[playerId]->generate(0);
                }
            }
            int det_stance[5][5];
            for (int i = 1; i <= 4; i++) {
                for (int j = 1; j <= 4; j++) {
                    det_stance[i][j] = 0;
                }
            }
            for (int i = 1; i <= 4; i++) {
                int tmp = getStance(i);
                for (int j = 1; j <= 4; j++) {
                    if (pre_score[i][j] != board[i]->getScore(j)) {
                        det_stance[j][tmp] += board[i]->getScore(j) - pre_score[i][j];
                    }
                }
            }
            for (int i = 1; i <= 4; i++) {
                for (int j = 1; j <= 4; j++) {
                    stance[i][j] += det_stance[i][j];
                }
            }
            cmd = QJsonObject();
            cmd["command"] = "status";
            QJsonObject parameter;
            QJsonArray stances;
            for (int i = 1; i <= 4; i++) {
                QJsonArray stance;
                for (int j = 1; j <= 4; j++) {
                    stance.append(this->stance[i][j]);
                }
                stances.append(stance);
            }
            parameter["stances"] = stances;
            cmd["parameter"] = parameter;
            QJsonDocument jsonOut(cmd);
            broadcast(jsonOut.toJson());
        });
    }
}
