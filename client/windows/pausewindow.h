#ifndef PAUSEWINDOW_H
#define PAUSEWINDOW_H
#include "framelesswindow.h"
namespace Ui {
class PauseWindow;
}

class PauseWindow : public FrameLessWindow {
    Q_OBJECT

public:
    explicit PauseWindow(QWidget *parent = nullptr);
    ~PauseWindow();

private slots:
    void on_btnReturn_clicked();
    void on_btnConit_clicked();

private:
    Ui::PauseWindow *ui;
};

#endif  // ABOUTWINDOW_H
