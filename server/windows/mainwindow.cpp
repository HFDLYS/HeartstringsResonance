#include "mainwindow.h"
#include "ui_mainwindow.h"

const int MULTI_SERVER_PORT=1479;
const int SOLO_SERVER_PORT=1478;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    webMultiServer = new QWebSocketServer("Multi-Game Server",QWebSocketServer::NonSecureMode,this);
    if(webMultiServer->listen(QHostAddress::Any,MULTI_SERVER_PORT))
    {
        qDebug()<<"多人游戏服务器开始监听";
        ui->textEdit->append(QString("多人服务启动,开始监听端口%1").arg(MULTI_SERVER_PORT));
        connect(webMultiServer,&QWebSocketServer::newConnection,this,&MainWindow::newClientConnect);
    }
    webSoloServer = new QWebSocketServer("Solo-Game Server",QWebSocketServer::NonSecureMode,this);
    if(webMultiServer->listen(QHostAddress::Any,SOLO_SERVER_PORT))
    {
        qDebug()<<"单人记录更新服务器开始监听";
        ui->textEdit->append(QString("单人服务启动,开始监听端口%1").arg(SOLO_SERVER_PORT));
        connect(webMultiServer,&QWebSocketServer::newConnection,this,[&]{
            auto client =webMultiServer->nextPendingConnection();
            connect(client,&QWebSocket::binaryMessageReceived,this,[=](const QByteArray &message){
                QJsonDocument jsonIn=QJsonDocument::fromJson(message);
                QJsonObject cmd=jsonIn.object();
                if(cmd["command"].toString()=="updatePoint"){
                    QJsonObject parameter=cmd["parameter"].toObject();
                    db.updateSolo(parameter["userName"].toString(),parameter["point"].toInt());
                }
            });
        });
    }
}
void MainWindow::newClientConnect(){
    auto client =webMultiServer->nextPendingConnection();
    qDebug()<<"有新的连接.";
    ui->textEdit->append(QString("有来自%1:%2的连接").arg(client->peerAddress().toString()).arg(client->peerPort()));
    waitingQueue.push_back(client);
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
        connect(room,&Room::updatePoint,this,[&](QString userName,int point){
            db.updateMulti(userName,point);
        });
        rooms.push_back(room);
        room->start();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exitBtn_clicked()
{
    exit(0);
}

