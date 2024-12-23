#ifndef MEMBERWINDOW_H
#define MEMBERWINDOW_H

#include <QWidget>
#include "basewindow.h"

namespace Ui {
class MemberWindow;
}

class MemberWindow : public BaseWindow
{
    Q_OBJECT

public:
    explicit MemberWindow(QWidget *parent = nullptr);
    ~MemberWindow();

private slots:
    void on_btnReturn_clicked();

private:
    Ui::MemberWindow *ui;
};

#endif // MEMBERWINDOW_H
