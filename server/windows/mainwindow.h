#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QQueue>
#include <QVector>
#include <QMainWindow>
#include <QWebSocketServer>
#include <QWebSocket>
#include "../serve/room.h"
#include "../serve/database.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_exitBtn_clicked();
    void newClientConnect();

private:
    DataBase db;
    Ui::MainWindow *ui;
    QWebSocketServer* webServer;
    QWebSocketServer* webSoloServer;
    QVector<QPair<QWebSocket*,QString> > waitingQueue;
    QVector<Room*> rooms;
};

#endif // MAINWINDOW_H
