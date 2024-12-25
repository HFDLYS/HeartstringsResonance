#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QTime>
#include <QWebSocket>
#include "pausewindow.h"
// #include "../core/board.h"
#include "../graphics/rendermanager.h"
// #include "recordrankwindow.h"
#include "basewindow.h"
#include "../../core/board.h"
#include "../component/squarechart.h"
// 游戏窗口类

namespace Ui {
class GameWindow;
}

class GameWindow : public BaseWindow {
    Q_OBJECT

signals:
    void gameStart();
    void wait(int);
public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
    bool is_connected() { return server->isValid(); }

public slots:
    void startGame();
    void Release1();
    void Release2();
    void Release3();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *e) override;

private:
    int difficulty_ = 1;

private slots:
    void on_skill1_button_clicked();

    void on_skill2_button_clicked();

    void on_skill3_button_clicked();

    void on_pause_button_clicked();

private:
    Ui::GameWindow *ui;
    int skill1_cnt = 0;
    int skill2_cnt = 0;
    int skill3_cnt = 0;
    QTimer *timer_flush_score_and_left_time_bar_;
    int left_time_cnt_;
    int player_id_;
    QVector<Player> players;
    // Board *board;
    Graphics::RenderManager *main_renderer_;
    Board *main_board_;
    SquarePieChart *main_chart_;
    Graphics::RenderManager *sub_renderer_;
    SquarePieChart *sub_chart_;
    Graphics::RenderManager *show_renderer_[5];
    Graphics::RenderManager *show_sub_renderer_[5];
    SquarePieChart *show_sub_chart_[5];
    Board *show_board_[5];
    SquarePieChart *show_chart_[5];
    QWebSocket *server;

    // RecordRankWindow *record_rank_window;  // 用作排行榜输入窗口
    bool is_pause;
    PauseWindow *pw;
    int seed=0;
    bool has_started_ = false;

    bool is_pausing_;  // 这个变量记录当前是否为暂停状态。游戏是否暂停，在项目中有三个不同变量记录它，注意数据同步问题！！！

public:

};

#endif  // GAMEWINDOW_H
