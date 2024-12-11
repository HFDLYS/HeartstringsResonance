#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCoreApplication>
#include <QEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QScreen>
#include <QTime>
#include <QTimer>

#include "framelesswindow.h"

/*图像规格:
    总要求:格式为png;
    (一)按钮
        0.所有按钮要常态和按下两种图;
        1.单人按钮和多人按钮:220*50;
        2.开始&排行&设置:400*60;
        3.退出&关于:300*50(两个图案左右对称);
    (二)背景
        大小1280*720;
        不需标题等布局要素;
*/

// 主菜单类

namespace Ui {
class MainWindow;
}

class MainWindow : public FrameLessWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnRank_clicked();

    void on_btnConfig_clicked();

    void on_btnAbout_clicked();

    void on_btnQuit_clicked();

    void on_btnRank_pressed();

    void on_btnRank_released();

    void on_btnConfig_pressed();

    void on_btnConfig_released();

    void on_rbtnSolo_clicked();

    void on_rbtnMultiplayer_clicked();

signals:
    void sentDifficulty(QString data);

private:
    Ui::MainWindow *ui;
    QString msg;
};

// 延迟时间，为优化切换窗口体验
void delay(int x);

#endif  // MAINWINDOW_H
