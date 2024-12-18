#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QTime>
#include <QWebSocket>

 #include "../../core/board.h"
#include "../graphics/rendermanager.h"
// #include "recordrankwindow.h"
#include "basewindow.h"
// 游戏窗口类

namespace Ui {
class GameWindow;
}

class GameWindow : public BaseWindow {
    Q_OBJECT

signals:
    void gameStart();

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

public slots:
    void startGame();
    void Release1();
    void Release2();
    void Release3();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QPoint last;

    Board *board[10];
    Graphics::RenderManager *renderer_[10];

private slots:
    void on_skill1_button_clicked();

    void on_skill2_button_clicked();

    void on_skill3_button_clicked();

    void on_pause_button_clicked();

private:
    Ui::GameWindow *ui;

    QTimer *timer_flush_score_and_left_time_bar_;
    int left_time_cnt_;

    // Board *board;
    Graphics::RenderManager *main_renderer_;
    Graphics::RenderManager *sub_renderer_[4];

    QWebSocket *server;

    // RecordRankWindow *record_rank_window;  // 用作排行榜输入窗口

    int seed=0;

    bool is_pausing_;  // 这个变量记录当前是否为暂停状态。游戏是否暂停，在项目中有三个不同变量记录它，注意数据同步问题！！！

public:
    void initBoard();
};

#endif  // GAMEWINDOW_H
