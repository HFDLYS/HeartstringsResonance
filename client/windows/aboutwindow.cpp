#include "aboutwindow.h"
#include "mainwindow.h"
#include "ui_aboutwindow.h"
#include "../audio/audiomanager.h"
#include "memberwindow.h"
#include "../config/globalconfig.h"
#include <QTime>

int AboutWindow::now_page = 0;
AboutWindow::AboutWindow(QWidget *parent) : BaseWindow(parent), ui(new Ui::AboutWindow) {
    ui->setupUi(this);
    QString part1 = ":/images/singlewindow/background";
    QString picid = QString::number(GlobalConfig::getInstance().getPicStyle() + 1);
    QString root = part1.append(picid)+ ".png";
    ui->aboutpic->setStyleSheet(
        QString("#aboutpic{border-image:url(%1)}").arg(root)
        );
    if (now_page == 3) {
        ui->btnNext->setVisible(false);
    }
    ui->about->setStyleSheet(
        QString("#about{border-image:url(:/images/aboutwindow/about" + QString::number(now_page + 1) + ".png)}")
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
    if (now_page != 0) {
        if (now_page == 3)
            ui->btnNext->setVisible(true);
        now_page--;
        QString part1 = ":/images/aboutwindow/about";
        QString picid = QString::number(now_page + 1);
        QString root = part1.append(picid)+ ".png";
        ui->about->setStyleSheet(
            QString("#about{border-image:url(%1)}").arg(root)
            );
        return;
    }
    changeWindow(new MainWindow());
}

void AboutWindow::on_btnMember_clicked() {
    AudioManager::GetInstance()->PlayLabel();
    MemberWindow *mbw = new MemberWindow();
    changeWindow(mbw);
}

void AboutWindow::on_btnNext_clicked() {
    AudioManager::GetInstance()->PlayLabel();
    now_page++;
    if(now_page==3) {
        ui->btnNext->setVisible(false);
    }
    QString part1 = ":/images/aboutwindow/about";
    QString picid = QString::number(now_page + 1);
    QString root = part1.append(picid)+ ".png";
    ui->about->setStyleSheet(
        QString("#about{border-image:url(%1)}").arg(root)
        );
}

