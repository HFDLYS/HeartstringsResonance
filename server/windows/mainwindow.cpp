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
        qDebug()<<"开始监听";
        ui->textEdit->append(QString("服务启动,开始监听端口%1").arg(SERVER_PORT));
        connect(webServer,&QWebSocketServer::newConnection,this,&MainWindow::newClientConnect);

    }
}
void MainWindow::newClientConnect(){
    auto client =webServer->nextPendingConnection();
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
        connect(room,&Room::broadcastSignal,this,[=](QString info){
            qDebug()<<"广播消息.";
            ui->textEdit->append(QString("房间%1广播了消息:%2").arg(room->getId()).arg(info));
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

