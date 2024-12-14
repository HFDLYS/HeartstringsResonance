#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QWidget>

namespace Ui {
class ResultWindow;
}

class ResultWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ResultWindow(QWidget *parent = nullptr);
    ~ResultWindow();

private slots:
    void on_btnReturn_clicked();

signals:
    void exitwindow();

private:
    Ui::ResultWindow *ui;
};

#endif // RESULTWINDOW_H
