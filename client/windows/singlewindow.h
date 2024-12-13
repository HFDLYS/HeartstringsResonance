#ifndef SINGLEWINDOW_H
#define SINGLEWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QTime>

// #include "../core/board.h"
#include "../graphics/rendermanager.h"
// #include "recordrankwindow.h"
#include "basewindow.h"
// 游戏窗口类

namespace Ui {
class SingleWindow;
}

class SingleWindow : public BaseWindow {
    Q_OBJECT

public:
    explicit SingleWindow(QWidget *parent = nullptr);
    ~SingleWindow();

public slots:
    void startGame();
    void Release1();
    void Release2();
    void Release3();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

private:
    QPoint last;
    int difficulty_ = 1;

private slots:
    void on_btnReturn_clicked();

    void on_skill1_button_clicked();

    void on_skill2_button_clicked();

    void on_skill3_button_clicked();

    void on_pause_button_clicked();

    void on_hint_button_clicked();

    void on_hint_button_pressed();

    void on_hint_button_released();

    void on_btnReturn_pressed();

    void on_btnReturn_released();

private:
    Ui::SingleWindow *ui;

    QTimer *timer_flush_score_and_left_time_bar_;
    int left_time_cnt_;

    // Board *board;
    Graphics::RenderManager *renderer_;

    // RecordRankWindow *record_rank_window;  // 用作排行榜输入窗口

    bool
        is_pausing_;  // 这个变量记录当前是否为暂停状态。游戏是否暂停，在项目中有三个不同变量记录它，注意数据同步问题！！！

public:
    void InitBoard();
    void RefreshTimeLabel();
};

#endif  // SINGLEWINDOW_H
