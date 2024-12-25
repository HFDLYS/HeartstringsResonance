#ifndef PAUSEWINDOW_H
#define PAUSEWINDOW_H
#include "basewindow.h"
namespace Ui {
class PauseWindow;
}

class PauseWindow : public QWidget {
    Q_OBJECT
public:
    void keyPressEvent(QKeyEvent *e);
public:
    explicit PauseWindow(int kind, QWidget *parent = nullptr);
    int gamekind;
    ~PauseWindow();

private slots:
    void on_btnReturn_clicked();
    void on_btnConti_clicked();

signals:
    void exitwindow();
    void gameContinue();

private:
    Ui::PauseWindow *ui;
};

#endif  // ABOUTWINDOW_H
