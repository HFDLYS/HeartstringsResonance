#ifndef SINGLEWINDOW_H
#define SINGLEWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QTime>
#include "pausewindow.h"
#include "../../core/board.h"
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
    explicit SingleWindow(int,int,QWidget *parent = nullptr);
    ~SingleWindow();

public slots:
    void startGame();
    void Release1();
    void Release2();
    void Release3();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    //void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e);

private:
    QTimer*timer;
    int max_gem_type = 5;
    int seed=0;
    int difficulty = 1;
    int skill1_cnt = 0;
    int skill2_cnt = 0;
    int skill3_cnt = 0;
private slots:
    //void on_btnReturn_clicked();

    void on_skill1_button_clicked();

    void on_skill2_button_clicked();

    void on_skill3_button_clicked();

    void on_pause_button_clicked();



private:
    Ui::SingleWindow *ui;
    bool has_started_=false;
    bool is_pause=false;
    QTimer *timer_flush_score_and_left_time_bar_;
    int left_time_cnt_;
    PauseWindow *pw;
    Board *board;
    Graphics::RenderManager *renderer_;
    bool is_brower_opened = false;
    // RecordRankWindow *record_rank_window;  // 用作排行榜输入窗口

    bool
        is_pausing_;  // 这个变量记录当前是否为暂停状态。游戏是否暂停，在项目中有三个不同变量记录它，注意数据同步问题！！！
    //上面这啥????

public:
    void initBoard();
    void refreshTimeLabel();
};

#endif  // SINGLEWINDOW_H
