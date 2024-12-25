#include "pausewindow.h"
#include "mainwindow.h"
#include "ui_pausewindow.h"
#include "../audio/audiomanager.h"
PauseWindow::PauseWindow(int kind, QWidget *parent) : QWidget(parent),ui(new Ui::PauseWindow),gamekind(kind){
    ui->setupUi(this);
    // 固定窗口大小
}


PauseWindow::~PauseWindow() {  }

void PauseWindow::on_btnReturn_clicked(){
    emit exitwindow();
}
void PauseWindow::keyPressEvent(QKeyEvent *e) {
    qDebug()<<"Pause";
    if(e->key()==Qt::Key_Escape){
        on_btnConti_clicked();
    }
}
void PauseWindow::on_btnConti_clicked()
{
    emit gameContinue();
    if(gamekind == 1)AudioManager::GetInstance()->ContinueBgm2();
    if(gamekind == 2)AudioManager::GetInstance()->ContinueBgm3();
    delete this;
}

