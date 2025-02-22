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
protected:
    void keyPressEvent(QKeyEvent *e);

private slots:
    void on_btnRank_clicked();

    void on_btnConfig_clicked();

    void on_btnAbout_clicked();

    void on_btnQuit_clicked();

    void on_rbtnSolo_clicked();

    void on_rbtnMultiplayer_clicked();

    void showEasy();
    void showMedium();
    void showHard();

    void on_btnBack_clicked();

signals:

private:
    Ui::MainWindow *ui;
    QString msg;
    QString ip;
    QString port;
public:
    QString getip();
    QString getport();
};

#endif  // MAINWINDOW_H
