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

void MemberWindow::keyPressEvent(QKeyEvent *e) {
    if(e->key()==Qt::Key_Escape){
        on_btnReturn_clicked();
    }
}
void MemberWindow::on_btnReturn_clicked()
{
    AudioManager::GetInstance()->PlayLabel();
    AboutWindow *flw = new AboutWindow();
    changeWindow(flw);
}

