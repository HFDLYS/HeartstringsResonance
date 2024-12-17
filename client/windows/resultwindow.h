#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QWidget>
#include "basewindow.h"

namespace Ui {
class ResultWindow;
}

class ResultWindow : public BaseWindow
{
    Q_OBJECT

public:
    explicit ResultWindow(int score, int score1, int score2, int score3, int score4, int score5, QWidget *parent = nullptr);
    ~ResultWindow();

private slots:
    void on_btnReturn_clicked();

    void on_btnUpdate_clicked();

signals:
    void exitwindow();

private:
    Ui::ResultWindow *ui;
};

#endif // RESULTWINDOW_H
