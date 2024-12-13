#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCoreApplication>
#include <QEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QScreen>
#include <QSurfaceFormat>

#include "basewindow.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public BaseWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnRank_clicked();

    void on_btnConfig_clicked();

    void on_btnAbout_clicked();

    void on_btnQuit_clicked();

    void on_rbtnSolo_clicked();

    void on_rbtnMultiplayer_clicked();

signals:

private:
    Ui::MainWindow *ui;
    QString msg;
};

#endif  // MAINWINDOW_H
