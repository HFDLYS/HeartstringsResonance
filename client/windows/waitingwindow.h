#ifndef WAITINGWINDOW_H
#define WAITINGWINDOW_H

#include <QWidget>

namespace Ui {
class WaitingWindow;
}

class WaitingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit WaitingWindow(QWidget *parent = nullptr);
    ~WaitingWindow();

private:
    Ui::WaitingWindow *ui;
};

#endif // WAITINGWINDOW_H
