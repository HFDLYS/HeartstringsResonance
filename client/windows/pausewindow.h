#ifndef PAUSEWINDOW_H
#define PAUSEWINDOW_H
#include "basewindow.h"
namespace Ui {
class PauseWindow;
}

class PauseWindow : public QWidget {
    Q_OBJECT

public:
    explicit PauseWindow(QWidget *parent = nullptr);
    ~PauseWindow();

private slots:
    void on_btnReturn_clicked();
    void on_btnConit_clicked();

    void on_btnConti_clicked();

signals:
    void exitwindow();

private:
    Ui::PauseWindow *ui;
};

#endif  // ABOUTWINDOW_H
