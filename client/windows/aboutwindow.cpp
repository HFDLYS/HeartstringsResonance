#include "aboutwindow.h"
#include "mainwindow.h"
#include "ui_aboutwindow.h"
#include "../audio/audiomanager.h"
#include "memberwindow.h"
#include "../config/globalconfig.h"
#include <QTime>
AboutWindow::AboutWindow(QWidget *parent) : BaseWindow(parent), ui(new Ui::AboutWindow) {
    ui->setupUi(this);
    QString part1 = ":/images/singlewindow/background";
    QString picid = QString::number(GlobalConfig::getInstance().getPicStyle() + 1);
    QString root = part1.append(picid)+ ".png";
    ui->aboutpic->setStyleSheet(
        QString("#aboutpic{border-image:url(%1)}").arg(root)
        );
}

void AboutWindow::keyPressEvent(QKeyEvent *e) {
    if(e->key()==Qt::Key_Escape){
        on_btnReturn_clicked();
    }
}
AboutWindow::~AboutWindow() { delete ui; }

void AboutWindow::on_btnReturn_clicked() {
    AudioManager::GetInstance()->PlayLabel();
    changeWindow(new MainWindow());
}

void AboutWindow::on_btnMember_clicked()
{
    AudioManager::GetInstance()->PlayLabel();
    MemberWindow *mbw = new MemberWindow();
    changeWindow(mbw);
}

