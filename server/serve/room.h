#ifndef ROOM_H
#define ROOM_H
#include "../../core/board.h"
#include "servergemmanager.h"
#include "../../core/player.h"
#include <QQueue>
#include <QVector>
#include <QMainWindow>
#include <QWebSocket>
#include <QThread>
#include <QRandomGenerator>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QTimer>
class Room : public QThread {
    Q_OBJECT
signals:
    void updatePoint(QString userName, int point);
    void sendMessage(const QByteArray& info, QWebSocket*);
public:
    Room(int, QPair<QWebSocket*, Player>, QPair<QWebSocket*, Player>, QPair<QWebSocket*, Player>, QPair<QWebSocket*, Player>, QObject* parent = nullptr);
    int getId();
protected:
    void run() override;
private:
    int id;
    int seed[5];
    QWebSocket* player[5];
    Player players[5];
    Board* board[5];
    QTimer* timer;
    int time;
    ServerGemManager* gem_manager;
    int stance[5][5];
    void broadcast(const QByteArray& info);
    void broadcast(const QByteArray& info, QWebSocket*);
    void broadcastStatus();
    int getStance(int ind);
};

#endif // ROOM_H
