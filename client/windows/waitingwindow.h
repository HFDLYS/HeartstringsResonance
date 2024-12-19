#ifndef WAITINGWINDOW_H
#define WAITINGWINDOW_H

#include <QWidget>
namespace Ui {
class WaitingWindow;
}

class WaitingWindow : public QWidget
{
    Q_OBJECT
signals:
    void closeGame();
public:
    explicit WaitingWindow(QWidget *parent = nullptr);
    ~WaitingWindow();

private slots:
    void on_btnExit_clicked();

private:
    Ui::WaitingWindow *ui;
};

#endif // WAITINGWINDOW_H
