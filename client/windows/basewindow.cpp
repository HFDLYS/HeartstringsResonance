#include "basewindow.h"

#include <QTime>
#include <QTimer>
#include "ui_basewindow.h"

BaseWindow::BaseWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::BaseWindow) {
    ui->setupUi(this);
    this->setFixedSize(1280, 720);
    this->setWindowFlag(Qt::FramelessWindowHint);
    QIcon icon = QIcon(":/images/windowicon.png");
    this->setWindowIcon(icon);
    QPixmap pix(":/images/mouse.png");
    QSize size(40, 40);
    // 设置图片大小
    pix = pix.scaled(size, Qt::KeepAspectRatio);
    this->setCursor(QCursor(pix, 12, 12));
}

void BaseWindow::delay(int x) {
    QTime dieTime = QTime::currentTime().addMSecs(x);
    while (QTime::currentTime() < dieTime) QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void BaseWindow::changeWindow(BaseWindow*flw){
    flw->move(this->pos().x(), this->pos().y());
    flw->show();
    delay(20);
    this->close();
}

const int TITLE_HEIGHT = 30;

void BaseWindow::mousePressEvent(QMouseEvent *e) {
    if (e->y() < TITLE_HEIGHT) {
        last = e->globalPos();
    }
}

/*
 * 鼠标移动函数
 * 这里实时修改窗口的坐标
 */
void BaseWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->y() < TITLE_HEIGHT) {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        this->move(this->x() + dx, this->y() + dy);
    }
}
/*
 * 鼠标释放函数
 */
void BaseWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->y() < TITLE_HEIGHT) {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        this->move(this->x() + dx, this->y() + dy);
    }
}

BaseWindow::~BaseWindow() { delete ui; }
