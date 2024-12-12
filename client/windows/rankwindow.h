#ifndef RANKWINDOW_H
#define RANKWINDOW_H

#include <utility>
#include <vector>
#include "basewindow.h"

namespace Ui {
class RankWindow;
}

class RankWindow : public BaseWindow {
    Q_OBJECT
private:
    std::vector<std::pair<std::string, int>> pairs;

public:
    explicit RankWindow(QWidget *parent = nullptr);
    ~RankWindow();

private slots:

    void on_btnReturn_clicked();

private:
    Ui::RankWindow *ui;
};

#endif  // RANKWINDOW_H
