#include "memberwindow.h"
#include "ui_memberwindow.h"
#include "aboutwindow.h"
#include "../config/globalconfig.h"
#include <QTime>
MemberWindow::MemberWindow(QWidget *parent)
    : BaseWindow(parent)
    , ui(new Ui::MemberWindow)
{
    ui->setupUi(this);
    QString part1 = ":/images/singlewindow/background";
    QString picid = QString::number(GlobalConfig::getInstance().getPicStyle() + 1);
    QString root = part1.append(picid)+ ".png";
    ui->memberpic->setStyleSheet(
        QString("#memberpic{border-image:url(%1)}").arg(root)
        );
     this->setWindowFlags(Qt::FramelessWindowHint);
}

MemberWindow::~MemberWindow()
{
    delete ui;
}

void MemberWindow::on_btnReturn_clicked()
{
    AudioManager::GetInstance()->PlayLabel();
    AboutWindow *flw = new AboutWindow();
    changeWindow(flw);
}

