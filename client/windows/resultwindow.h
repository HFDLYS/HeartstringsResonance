#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QWidget>
#include <QWebSocket>
#include "basewindow.h"
#include "../graphics/rendermanager.h"

namespace Ui {
class ResultWindow;
}

class ResultWindow : public BaseWindow
{
    Q_OBJECT

public:
    explicit ResultWindow(bool isSolo_,int score, int score1, int score2, int score3, int score4, int score5, QWidget *parent = nullptr);
    ~ResultWindow();

    void showGem();
private slots:
    void on_btnReturn_clicked();

    void on_btnUpdate_clicked();

signals:
    void exitwindow(QVector<QMetaObject::Connection>);
private:
    bool isSolo;
    int score;
    QWebSocket*server;
    Ui::ResultWindow *ui;
    Graphics::RenderManager *gem_render_[5];
};

#endif // RESULTWINDOW_H
