#ifndef MultiResultWindow_H
#define MultiResultWindow_H

#include <QWidget>
#include <QWebSocket>
#include "basewindow.h"
#include "../graphics/rendermanager.h"

namespace Ui {
class MultiResultWindow;
}

class MultiResultWindow : public BaseWindow
{
    Q_OBJECT
protected:
    void keyPressEvent(QKeyEvent *e);
public:
    explicit MultiResultWindow(QJsonObject parameter,int playerId, QWidget *parent = nullptr);
    ~MultiResultWindow();

    void showGem();
private slots:
    void on_btnReturn_clicked();

    void on_btnUpdate_clicked();

signals:
    void exitwindow(QVector<QMetaObject::Connection>);
private:
    bool isSolo=0;
    int score;
    QWebSocket*server;
    Ui::MultiResultWindow *ui;
    Graphics::RenderManager *gem_render_[5];
    Graphics::RenderManager *my_gem_render_[5];
};

#endif // MultiResultWindow_H
