#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QQueue>
#include <QVector>
#include <QMainWindow>
#include <QWebSocketServer>
#include <QWebSocket>
#include "../serve/room.h"
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
    Ui::MainWindow *ui;
    QWebSocketServer* webServer;
    QQueue<QWebSocket*> waitingQueue;
    QVector<Room*> rooms;
};

#endif // MAINWINDOW_H
