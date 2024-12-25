#include "mainwindow.h"
#include "ui_mainwindow.h"

const int SERVER_PORT = 1479;
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    webServer = new QWebSocketServer("Game Server", QWebSocketServer::NonSecureMode, this);
    if (webServer->listen(QHostAddress::Any, SERVER_PORT)) {
        qDebug() << "游戏服务器开始监听";
        ui->textEdit->append(QString("服务启动,开始监听端口%1").arg(SERVER_PORT));
        connect(webServer, &QWebSocketServer::newConnection, this, &MainWindow::newClientConnect);
    }
}
void MainWindow::newClientConnect() {
    auto client = webServer->nextPendingConnection();
    ui->textEdit->append(QString("有来自%1:%2的连接").arg(client->peerAddress().toString()).arg(client->peerPort()));
    qDebug() << QString("有来自%1:%2的连接").arg(client->peerAddress().toString()).arg(client->peerPort());
    connect(client, &QWebSocket::disconnected, this, [=] {
        for (int i=0;i<waitingQueue.size();i++) {
            if (waitingQueue[i].first == client) {
                waitingQueue.removeAt(i);
                break;
            }
        }
        ui->textEdit->append(QString("与%1:%2的连接断开").arg(client->peerAddress().toString()).arg(client->peerPort()));
        qDebug() << QString("与%1:%2的连接断开").arg(client->peerAddress().toString()).arg(client->peerPort());
        for(auto a:waitingQueue){
            QJsonObject cmdOut,parameterOut;
            cmdOut["command"]="waiting";
            parameterOut["num"]=waitingQueue.size();
            cmdOut["parameter"]=parameterOut;
            QJsonDocument jsonOut(cmdOut);
            a.first->sendBinaryMessage(jsonOut.toJson());
        }
        qDebug() << "当前等待人数:" << waitingQueue.size();
        });
    connect(client, &QWebSocket::binaryMessageReceived, this, [=](const QByteArray& message) {
        QJsonDocument jsonIn = QJsonDocument::fromJson(message);
        ui->textEdit->append(QString("收到%1:%2的信息:%3").arg(client->peerAddress().toString()).arg(client->peerPort()).arg(message));
        qDebug() << QString("收到%1:%2的信息:%3").arg(client->peerAddress().toString()).arg(client->peerPort()).arg(message);
        QJsonObject cmd = jsonIn.object();
        if (cmd["command"].toString() == "updatePoint") {
            QJsonObject parameter = cmd["parameter"].toObject();
            int skill1 = parameter["pointSolo"].toInt()/50+1;
            int skill2 = parameter["pointSolo"].toInt()/200+1;
            int skill3 = parameter["pointSolo"].toInt()/100+1;
            auto player=db.update(parameter["userName"].toString(), parameter["pointSolo"].toInt(), parameter["pointMulti"].toInt(), skill1, skill2, skill3);
            QJsonObject cmdOut,parameterOut;
            cmdOut["command"]="updateSkill";
            parameterOut["player"]=player[0].toJson();
            cmdOut["parameter"]=parameterOut;
            QJsonDocument jsonOut(cmdOut);
            client->sendBinaryMessage(jsonOut.toJson());
        }else if (cmd["command"].toString() == "cancelmulti") {
            for (int i=0;i<waitingQueue.size();i++) {
                if (waitingQueue[i].first == client) {
                    waitingQueue.removeAt(i);
                    break;
                }
            }
            for(auto a:waitingQueue){
                QJsonObject cmdOut,parameterOut;
                cmdOut["command"]="waiting";
                parameterOut["num"]=waitingQueue.size();
                cmdOut["parameter"]=parameterOut;
                QJsonDocument jsonOut(cmdOut);
                a.first->sendBinaryMessage(jsonOut.toJson());
            }
        } else if (cmd["command"].toString() == "rank") {
            /**/
            QJsonObject parameter = cmd["parameter"].toObject();
            auto soloRank = db.rankSolo(parameter["length"].toInt());
            QJsonArray soloArray;
            for (auto soloPlayer : soloRank) {
                QJsonObject player;
                player["userName"] = soloPlayer.first;
                player["point"] = soloPlayer.second;
                soloArray.append(player);
            }
            /**/
            auto multiRank = db.rankMulti(parameter["length"].toInt());
            QJsonArray multiArray;
            for (auto multiPlayer : multiRank) {
                QJsonObject player;
                player["userName"] = multiPlayer.first;
                player["point"] = multiPlayer.second;
                multiArray.append(player);
            }
            QJsonObject parameterOut;
            parameterOut["soloRank"] = soloArray;
            parameterOut["multiRank"] = multiArray;
            QJsonObject cmdOut;
            cmdOut["command"] = "rank";
            cmdOut["parameter"] = parameterOut;
            QJsonDocument jsonOut(cmdOut);
            client->sendBinaryMessage(jsonOut.toJson());
        } else if (cmd["command"].toString() == "multigame") {
            QJsonObject parameter = cmd["parameter"].toObject();
            //QString username = parameter["username"].toString();
            Player player(parameter["player"].toObject());
            waitingQueue.push_back(qMakePair(client, player));
            qDebug() << "当前等待人数:" << waitingQueue.size();
            if (waitingQueue.size() >= 4) {
                auto client1 = waitingQueue.front();
                waitingQueue.pop_front();
                auto client2 = waitingQueue.front();
                waitingQueue.pop_front();
                auto client3 = waitingQueue.front();
                waitingQueue.pop_front();
                auto client4 = waitingQueue.front();
                waitingQueue.pop_front();
                Room* room = new Room(rooms.size(), client1, client2, client3, client4, this);
                connect(room, &Room::sendMessage, this, [=](const QByteArray& info, QWebSocket* player) {
                    player->sendBinaryMessage(info);
                    ui->textEdit->append(QString("房间%1向%2:%3发送了消息:\n%4")
                        .arg(room->getId())
                        .arg(client->peerAddress().toString())
                        .arg(client->peerPort())
                        .arg(info));
                    });
                rooms.push_back(room);
                room->start();
            }else{
                for(auto a:waitingQueue){
                    QJsonObject cmdOut,parameterOut;
                    cmdOut["command"]="waiting";
                    parameterOut["num"]=waitingQueue.size();
                    cmdOut["parameter"]=parameterOut;
                    QJsonDocument jsonOut(cmdOut);
                    a.first->sendBinaryMessage(jsonOut.toJson());
                }
            }
        } else if (cmd["command"].toString() == "register") {
            QJsonObject parameter = cmd["parameter"].toObject();
            QString username = parameter["username"].toString();
            QString password = parameter["password"].toString();
            auto rec = db.playerRegister(username, password);
            bool isSuccess = rec.first;
            QString info = rec.second;
            QJsonObject parameterOut;
            parameterOut["isSuccess"] = isSuccess;
            parameterOut["info"] = info;
            QJsonObject cmdOut;
            cmdOut["command"] = "register";
            cmdOut["parameter"] = parameterOut;
            QJsonDocument jsonOut(cmdOut);
            client->sendBinaryMessage(jsonOut.toJson());
        } else if (cmd["command"].toString() == "login") {
            QJsonObject parameter = cmd["parameter"].toObject();
            QString username = parameter["username"].toString();
            QString password = parameter["password"].toString();
            Player player;
            auto rec = db.playerLogIn(username, password, player);
            bool isSuccess = rec.first;
            QString info = rec.second;
            QJsonObject parameterOut;
            parameterOut["isSuccess"] = isSuccess;
            parameterOut["info"] = info;
            parameterOut["player"] = player.toJson();
            QJsonObject cmdOut;
            cmdOut["command"] = "login";
            cmdOut["parameter"] = parameterOut;
            QJsonDocument jsonOut(cmdOut);
            client->sendBinaryMessage(jsonOut.toJson());
        }
        });

}
MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_exitBtn_clicked() {
    exit(0);
}

