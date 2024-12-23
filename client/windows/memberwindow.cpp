#include "memberwindow.h"
#include "ui_memberwindow.h"
#include "aboutwindow.h"
#include <QTime>
MemberWindow::MemberWindow(QWidget *parent)
    : BaseWindow(parent)
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
    changeWindow(flw);
}

