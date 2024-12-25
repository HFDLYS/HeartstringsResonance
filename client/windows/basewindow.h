#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include "../../core/player.h"
#include <QWebSocket>
// 无边窗口类，为其他窗口的父类

namespace Ui {
class BaseWindow;

}

class BaseWindow : public QMainWindow {
    Q_OBJECT

public:
    static void setPlayer(Player);
    static void setip(QString);
    static void setserver(QWebSocket *);
    explicit BaseWindow(QWidget *parent = nullptr);
    ~BaseWindow();
    void delay(int x);
    void changeWindow(BaseWindow*);
protected:
    QPoint last;
    static Player player;
    static QUrl playerIp;
    static QWebSocket *server;
    QVector<QMetaObject::Connection> connections;
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
private:
    Ui::BaseWindow *ui;
};

#endif  // BASEWINDOW_H
