#include <QInputDialog>
#include <QMessageBox>
#include "basewindow.h"
#include "resultwindow.h"
#include "ui_resultwindow.h"
#include "mainwindow.h"
#include "../audio/audiomanager.h"
ResultWindow::ResultWindow(int score, int score1, int score2, int score3, int score4, int score5, QWidget *parent)
    : BaseWindow(parent)
    , ui(new Ui::ResultWindow)
{
    ui->setupUi(this);
    ui->score->setText(QString::number(score));
    ui->cnt1->setText(QString::number(score1));
    ui->cnt2->setText(QString::number(score2));
    ui->cnt3->setText(QString::number(score3));
    ui->cnt4->setText(QString::number(score4));
    ui->cnt5->setText(QString::number(score5));
}

void ResultWindow::on_btnReturn_clicked(){
    emit exitwindow();
}

ResultWindow::~ResultWindow()
{
    delete ui;
}

void ResultWindow::on_btnUpdate_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this,
                                         "上传成绩",
                                         "请写下您的大名:",
                                         QLineEdit::Normal,
                                         "",
                                         &ok);

    if (ok && !text.isEmpty()) {
        //在这里补充数据库逻辑
        //
        //
        QMessageBox::information(this, "上传成功", text+"的成绩已上传");
         emit exitwindow();


    } else {
        QMessageBox::information(this, "取消", "我的名字呢？");
        emit exitwindow();
    }
}

