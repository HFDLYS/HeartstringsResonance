#ifndef WAITINGWINDOW_H
#define WAITINGWINDOW_H

#include <QWidget>
#include <QTimer>
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
    void keyPressEvent(QKeyEvent *e);
private slots:
    void on_btnExit_clicked();
    void updateLabels();

private:
    Ui::WaitingWindow *ui;
    int cnt = 0;
    QTimer *timer;
};

#endif // WAITINGWINDOW_H
