#ifndef RANKWINDOW_H
#define RANKWINDOW_H

#include <utility>
#include <vector>
#include <QWebSocket>
#include "basewindow.h"

namespace Ui {
class RankWindow;
}

class RankWindow : public BaseWindow {
    Q_OBJECT
private:
    std::vector<std::pair<std::string, int>> pairs;
protected:
    void keyPressEvent(QKeyEvent *e);
public:
    explicit RankWindow(QWidget *parent = nullptr);
    ~RankWindow();

private slots:

    void on_btnReturn_clicked();

private:
    Ui::RankWindow *ui;
    QWebSocket *server;
    QString id_solo[20];
    int score_solo[20] = {0};
    QString id_multi[20];
    int score_multi[20] = {0};
};

#endif  // RANKWINDOW_H
