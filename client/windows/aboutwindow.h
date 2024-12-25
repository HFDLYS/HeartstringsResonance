#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H
#include "basewindow.h"

// 关于窗口类

namespace Ui {
class AboutWindow;
}

class AboutWindow : public BaseWindow {
    Q_OBJECT

public:
    explicit AboutWindow(QWidget *parent = nullptr);
    ~AboutWindow();
protected:
    void keyPressEvent(QKeyEvent *e);
private slots:
    void on_btnReturn_clicked();

    void on_btnMember_clicked();

private:
    Ui::AboutWindow *ui;
};

#endif  // ABOUTWINDOW_H
