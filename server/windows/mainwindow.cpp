#include "mainwindow.h"
#include "ui_mainwindow.h"

const int SERVER_PORT=1479;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    webServer = new QWebSocketServer("Game Server",QWebSocketServer::NonSecureMode,this);
    if(webServer->listen(QHostAddress::Any,SERVER_PORT))
    {
        qDebug()<<"游戏服务器开始监听";
        ui->textEdit->append(QString("服务启动,开始监听端口%1").arg(SERVER_PORT));
        connect(webServer,&QWebSocketServer::newConnection,this,&MainWindow::newClientConnect);
    }
}
void MainWindow::newClientConnect(){
    auto client =webServer->nextPendingConnection();
    ui->textEdit->append(QString("有来自%1:%2的连接").arg(client->peerAddress().toString()).arg(client->peerPort()));
    qDebug()<<QString("有来自%1:%2的连接").arg(client->peerAddress().toString()).arg(client->peerPort());
    connect(client,&QWebSocket::disconnected,this,[=]{
        waitingQueue.removeAll(client);
        ui->textEdit->append(QString("与%1:%2的连接断开").arg(client->peerAddress().toString()).arg(client->peerPort()));
        qDebug()<<QString("与%1:%2的连接断开").arg(client->peerAddress().toString()).arg(client->peerPort());
        qDebug()<<"当前等待人数:"<<waitingQueue.size();
    });
    connect(client,&QWebSocket::binaryMessageReceived,this,[=](const QByteArray &message){
        QJsonDocument jsonIn=QJsonDocument::fromJson(message);
        ui->textEdit->append(QString("收到%1:%2的信息:%3").arg(client->peerAddress().toString()).arg(client->peerPort()).arg(message));
        qDebug()<<QString("收到%1:%2的信息:%3").arg(client->peerAddress().toString()).arg(client->peerPort()).arg(message);
        QJsonObject cmd=jsonIn.object();
        if(cmd["command"].toString()=="updatePoint"){
            QJsonObject parameter=cmd["parameter"].toObject();
            db.update(parameter["userName"].toString(),parameter["pointSolo"].toInt(),parameter["pointMulti"].toInt());
        }else if(cmd["command"].toString()=="rank"){
            QJsonObject parameter=cmd["parameter"].toObject();
            auto soloRank=db.rankSolo(parameter["length"].toInt());
            QJsonArray soloArray;
            for(auto soloPlayer:soloRank){
                QJsonObject player;
                player["userName"]=soloPlayer.first;
                player["point"]=soloPlayer.second;
                soloArray.append(player);
            }
            auto multiRank=db.rankMulti(parameter["length"].toInt());
            QJsonArray multiArray;
            for(auto multiPlayer:multiRank){
                QJsonObject player;
                player["userName"]=multiPlayer.first;
                player["point"]=multiPlayer.second;
                multiArray.append(player);
            }
            QJsonObject parameterOut;
            parameterOut["multiRank"]=multiArray;
            QJsonObject cmdOut;
            cmdOut["command"]="rank";
            cmdOut["parameter"]=parameterOut;
            QJsonDocument jsonOut(cmdOut);
            client->sendBinaryMessage(jsonOut.toJson());
        }else if(cmd["command"].toString()=="multigame"){
            waitingQueue.push_back(client);
            qDebug()<<"当前等待人数:"<<waitingQueue.size();
            if(waitingQueue.size()>=4){
                auto client1=waitingQueue.front();
                waitingQueue.pop_front();
                auto client2=waitingQueue.front();
                waitingQueue.pop_front();
                auto client3=waitingQueue.front();
                waitingQueue.pop_front();
                auto client4=waitingQueue.front();
                waitingQueue.pop_front();
                Room*room=new Room(rooms.size(),client1,client2,client3,client4,this);
                connect(room,&Room::sendMessage,this,[=](const QByteArray &info,QWebSocket*player){
                    player->sendBinaryMessage(info);
                    ui->textEdit->append(QString("房间%1向%2:%3发送了消息:\n%4")
                                             .arg(room->getId())
                                             .arg(client->peerAddress().toString())
                                             .arg(client->peerPort())
                                             .arg(info));
                });
                rooms.push_back(room);
                room->start();
            }
        }
    });

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exitBtn_clicked()
{
    exit(0);
}

