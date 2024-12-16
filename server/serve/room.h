#ifndef ROOM_H
#define ROOM_H
#include <QQueue>
#include <QVector>
#include <QMainWindow>
#include <QWebSocket>
#include <QThread>
class Room: public QThread
{
    Q_OBJECT
signals:
    void broadcastSignal(QString);
public:
    Room(int,QWebSocket*,QWebSocket*,QWebSocket*,QWebSocket*,QObject *parent = nullptr);
    int getId();
protected:
    void run() override;
private:
    int id;
    int seed;
    QWebSocket*player[5];
    void broadcast(QString);
};

#endif // ROOM_H
