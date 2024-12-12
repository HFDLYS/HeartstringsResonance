#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

// 无边窗口类，为其他窗口的父类

namespace Ui {
class FrameLessWindow;
}

class FrameLessWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit FrameLessWindow(QWidget *parent = nullptr);
    ~FrameLessWindow();
    void delay(int x);
    void changeWindow(FrameLessWindow*);
protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

private:
    QPoint last;

private:
    Ui::FrameLessWindow *ui;
};

#endif  // FRAMELESSWINDOW_H
