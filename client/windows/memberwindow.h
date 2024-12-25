#ifndef MEMBERWINDOW_H
#define MEMBERWINDOW_H

#include <QWidget>
#include "../audio/audiomanager.h"
#include "basewindow.h"

namespace Ui {
class MemberWindow;
}

class MemberWindow : public BaseWindow
{
    Q_OBJECT
protected:
    void keyPressEvent(QKeyEvent *e);

public:
    explicit MemberWindow(QWidget *parent = nullptr);
    ~MemberWindow();

private slots:
    void on_btnReturn_clicked();

private:
    Ui::MemberWindow *ui;
};

#endif // MEMBERWINDOW_H
