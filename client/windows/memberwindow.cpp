#include "memberwindow.h"
#include "ui_memberwindow.h"
#include "aboutwindow.h"
#include <QTime>
MemberWindow::MemberWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MemberWindow)
{
    ui->setupUi(this);
     this->setWindowFlags(Qt::FramelessWindowHint);
}

MemberWindow::~MemberWindow()
{
    delete ui;
}

void MemberWindow::on_btnReturn_clicked()
{
    AboutWindow *flw = new AboutWindow();
    flw->move(this->pos().x(), this->pos().y());
    flw->show();
    QTime dieTime = QTime::currentTime().addMSecs(20);
    while (QTime::currentTime() < dieTime) QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    this->close();
}

