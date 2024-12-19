#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

// 无边窗口类，为其他窗口的父类

namespace Ui {
class BaseWindow;
}

class BaseWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit BaseWindow(QWidget *parent = nullptr);
    ~BaseWindow();
    void delay(int x);
    void changeWindow(BaseWindow*);
protected:
    QPoint last;
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
private:
    Ui::BaseWindow *ui;
};

#endif  // BASEWINDOW_H
