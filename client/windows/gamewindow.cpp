#include "gamewindow.h"
#include "mainwindow.h"
#include "ui_gamewindow.h"
#include "pausewindow.h"
#include "waitingwindow.h"
#include "../audio/audiomanager.h"
#include "resultwindow.h"
#include <QAction>
#include <QBitmap>
#include <QDebug>
#include <QFont>
#include <QFontDatabase>
#include <QPicture>
#include <QPixmap>
#include <ctime>
#include <iostream>
#include <random>


/*
 * 未实现功能:
 * 1.等待界面
 * 2.交换宝石时发送信息至服务器.(格式:click/x1/y1/x2/y2)
 * 3.收到服务器交换棋子的信息(格式:click/x1/y1/x2/y2/playerId)时交换全局版面的对应宝石.
 * 4.开始游戏时将服务器返回的种子填入游戏.
*/

const QPoint board_size(500, 500);
const QPoint opengl_up_left(25, 100);
const QPoint opengl_down_right = opengl_up_left + QPoint(board_size.x(), board_size.y());
const int TITLE_HEIGHT = 30;
const int SERVER_PORT=1479;
const QUrl serverUrl("ws://localhost:1479");
GameWindow::GameWindow(QWidget *parent)
    : BaseWindow(parent), ui(new Ui::GameWindow) {
    ui->setupUi(this);
    server=new QWebSocket();
    server->open(serverUrl);
    //修改为等待界面
    WaitingWindow *ww = new WaitingWindow(this);
    ww->setGeometry(0,0,1280,720);
    ww->show();
    connect(server,&QWebSocket::textMessageReceived,this,[&](QString message){
        qDebug()<<message;
        QStringList info=message.split("/");
        if(info[0]=="start"){
            qDebug()<<info[1];
            seed=info[1].toInt();
            emit gameStart();//可能因不明原因崩溃.
            startGame();
        }else if(info[0]=="click"){
            //移动对应小棋盘的棋子
        }else if(info[0]=="end"){
            //结束游戏.
            AudioManager::GetInstance()->StopBgm2();
            ResultWindow *rw = new ResultWindow(this);
            rw->setGeometry(0,0,1280,720);
            rw->show();
            //changeWindow(new MainWindow());
            connect(rw, &ResultWindow::exitwindow, this, [this]{
                changeWindow(new MainWindow());
            });
        }
    });
    renderer_ = new Graphics::RenderManager(ui->controlWidget);
    renderer_->setFixedSize(board_size.x(), board_size.y());
    renderer_->setGeometry(opengl_up_left.x(), opengl_up_left.y(), renderer_->width(), renderer_->height());
    // QIcon icon = QIcon(":/images/windowicon.png");
    // this->setWindowIcon(icon);
    // QPixmap pix(":/images/mouse.png");
    // QSize size(24, 24);
    // // 设置图片大小
    // pix = pix.scaled(size, Qt::KeepAspectRatio);
    // this->setCursor(QCursor(pix, -1, -1));

    // QString ppath = QString("#aiwidget{border-image:url(:/images/立绘/ai%1.png)}")
    //                     .arg(rand() % 12 + 1);
    // qDebug() << ppath << endl;
    // ui->aiwidget->setStyleSheet(
    //     QString("#aiwidget{border-image:url(:/images/立绘/ai%2.png)}")
    //         .arg(rand() % 12 + 1));
    // // close时析构成员变量
    // // setAttribute(Qt::WA_DeleteOnClose);

    //   /*QPixmap pix;
    //       pix.load(":/images/gamewindow/1.png");
    //       ui->skill1_button->setFixedSize(pix.size());
    //       ui->skill1_button->setMask(pix.mask());
    //   */
    //   ui->skill1_button->setStyleSheet("background-color:rgba(0,0,0,0)");

    // // 创建Hypercube窗口
    // hypercube_ = new Hypercube::Hypercube(ui->aiwidget);
    // hypercube_->setFixedSize(hypercube_size.x(), hypercube_size.y());
    // hypercube_->setGeometry(opengl_up_left.x(), opengl_up_left.y(),
    //                         hypercube_->width(), hypercube_->height());

    //   // shadow
    //   ui->shadow->raise();

    //   // 初始化进度条
    //   is_pausing_ = false;
    //   // 创建timer
    //   timer_flush_score_and_left_time_bar_ = new QTimer(this);
    //   left_time_cnt_ = 12100;  // 1200* 0.1s
    //   connect(timer_flush_score_and_left_time_bar_, &QTimer::timeout, [&]() {
    //       // score_bar
    //       ui->score_bar->setText(QString::fromStdString(std::to_string(board->GetScore())));
    //       // left_time_bar
    //       if (is_pausing_ == false) left_time_cnt_ -= 10;
    //       RefreshTimeLabel();
    //       // set button state
    //       ui->skill1_button->setEnabled(board->GetRest1() > 0);
    //       ui->skill2_button->setEnabled(board->GetRest2() > 0);
    //       ui->skill3_button->setEnabled(board->GetRest3() > 0);
    //       ui->hint_button->setEnabled(board->GetScore() >= 100);
    //       // stop
    //       if (left_time_cnt_ <= 0 || board->IsGameOver()) {
    //           timer_flush_score_and_left_time_bar_->stop();
    //           left_time_cnt_ = -10;
    //           on_btnReturn_clicked();  // 时间到，直接退出游戏（
    //       }
    //   });
    //   timer_flush_score_and_left_time_bar_->start(100);

    //   // Record Rank Window
    //   record_rank_window = new RecordRankWindow(-1);

    //   int font_Id = QFontDatabase::addApplicationFont(":/font/SmileySans-Oblique.ttf");
    //   QStringList font_list = QFontDatabase::applicationFontFamilies(font_Id);
    //   qDebug() << font_Id;
    //   qDebug() << font_list;
    //   if (!font_list.isEmpty()) {
    //       QFont f;
    //       f.setFamily(font_list[0]);
    //       f.setPointSize(17);
    //       ui->score_bar->setFont(f);
    //       f.setPointSize(20);
    //       ui->minute_label->setFont(f);
    //       ui->minute_label->setStyleSheet("color:white;");
    //       ui->second_label->setFont(f);
    //       ui->second_label->setStyleSheet("color:white;");
    //   }
}

GameWindow::~GameWindow() {
    // // timer
    // timer_flush_score_and_left_time_bar_->stop();
    // // ui
    // delete ui;
    // // board
    // delete board;
}

void GameWindow::RefreshTimeLabel() {
    // int seconds = left_time_cnt_ / 100;
    // int minutes = seconds / 60;
    // seconds %= 60;
    // ui->minute_label->setText(QString::number(minutes));
    // QString s = "";
    // if (seconds < 10) s += "0";
    // s += QString::number(seconds);
    // ui->second_label->setText(s);
}

void GameWindow::InitBoard() {
    // // board
    // board = new Board(difficulty_);
    // board->SetHypercube(hypercube_);
    // board->InitHypercube();
    // // timer etc
    // left_time_cnt_ = 12100;
    // // connect
    // connect(board, SIGNAL(Release1()), this, SLOT(Release1()));
    // connect(board, SIGNAL(Release2()), this, SLOT(Release2()));
    // connect(board, SIGNAL(Release3()), this, SLOT(Release3()));
    // std::cerr << "GameWindow::GameWindow InitHypercube." << std::endl;
}

void GameWindow::Release1() { ui->skill1_button->setIcon(QIcon(":/images/gamewindow/1.png")); }
void GameWindow::Release2() { ui->skill2_button->setIcon(QIcon(":/images/gamewindow/2.png")); }
void GameWindow::Release3() { ui->skill3_button->setIcon(QIcon(":/images/gamewindow/3.png")); }

void GameWindow::mousePressEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();
    // std::cout << "mouse cliked on:" << x << " " << y << std::endl;
    if (event->y() < TITLE_HEIGHT) {
        last = event->globalPos();
    }
    // board->Clicked(x, y);
    if (x > opengl_up_left.x() && y > opengl_up_left.y() && x < opengl_down_right.x() && y < opengl_down_right.y()) {
        
        x -= opengl_up_left.x();
        y -= opengl_up_left.y();
        std::cout << "mouse cliked on:" << x << " " << y << std::endl;
        std::cout <<  8 * x /(board_size.x())*8 + 8 * y/(board_size.y())+1 << '\n';
        int nx = 8 * x/(board_size.x());
        int ny = 8 * y/(board_size.y());
        int nd = x /(board_size.x() / 8)*8 + y/(board_size.y() / 8)+1;
        renderer_->GetGemManager()->Remove(nd, true);
        // std::cout << nd << '\n';
    }
}
/*
 * 鼠标移动函数
 * 这里实时修改窗口的坐标
 */
void GameWindow::mouseMoveEvent(QMouseEvent *event) {
    // if (event->y() < TITLE_HEIGHT) {
    //     int dx = event->globalX() - last.x();
    //     int dy = event->globalY() - last.y();
    //     last = event->globalPos();
    //     this->move(this->x() + dx, this->y() + dy);
    // }
}
/*
 * 鼠标释放函数
 */
void GameWindow::mouseReleaseEvent(QMouseEvent *event) {
    // if (event->y() < TITLE_HEIGHT) {
    //     int dx = event->globalX() - last.x();
    //     int dy = event->globalY() - last.y();
    //     this->move(this->x() + dx, this->y() + dy);
    // }
}

/*
 * 键盘监听函数
 */
void GameWindow::keyPressEvent(QKeyEvent *e) {
    //   static bool shader_blinn_phong = true;
    //   static int shader_render_mode = 0;
    //   static int shader_light_source = 0;
    //   static float shader_hdr_exposure = 0.0;

    //   if (e->key() == Qt::Key_B) {  // B键，是否启用BlinnPhong光照模型
    //       shader_blinn_phong ^= 1;
    //       std::cout << "Blinn Phong: " << (shader_blinn_phong ? "true" : "false") << std::endl;
    //       hypercube_->SetBlinnPhong(shader_blinn_phong);
    //   }
    //   if (e->key() == Qt::Key_E || e->key() == Qt::Key_R) {
    //       if (e->key() == Qt::Key_E)
    //           shader_render_mode = (shader_render_mode + 4) % 5;  // E键，切换渲染模式，前翻
    //       else
    //           shader_render_mode = (shader_render_mode + 1) % 5;  // R键，切换渲染模式，后翻

    //   std::cout << "Render Mode: " << shader_render_mode << std::endl;
    //   hypercube_->SetRenderMode(shader_render_mode);
    // }
    // if (e->key() == Qt::Key_L) {  // L键，切换渲染光源
    //   shader_light_source = (shader_light_source + 1) % 3;
    //   std::cout << "Light Source: " << shader_light_source << std::endl;
    //   hypercube_->SetLightSource(shader_light_source);
    // }
    // if (e->key() == Qt::Key_G || e->key() == Qt::Key_H) {  // H键，切换HDR曝光等级
    //   shader_hdr_exposure += (e->key() == Qt::Key_H ? 1.f : -1.f) * 0.1f;
    //   std::cout << "HDR Exposure: " << shader_hdr_exposure << std::endl;
    //   hypercube_->SetHDRExposure(shader_hdr_exposure);
    // }
    // if (e->key() == Qt::Key_Shift) {
    //   on_pause_button_clicked();
    // }
    // if (e->key() == Qt::Key_A) {
    //   on_skill1_button_clicked();
    // }
    // if (e->key() == Qt::Key_S) {
    //   on_skill2_button_clicked();
    // }
    // if (e->key() == Qt::Key_D) {
    //   on_skill3_button_clicked();
    // }
    // // 彩蛋
    // if (e->key() == Qt::Key_O) {
    //   ui->aiwidget->setStyleSheet(
    //       "#aiwidget{border-image:url(:/images/立绘/nb.png)}");
    // }
}

void GameWindow::startGame() {

    renderer_->Demo();
}

// void GameWindow::on_btnReturn_clicked() {
//     MainWindow *mw = new MainWindow();
//     mw->move(this->pos().x(), this->pos().y());
//     mw->show();

//     // record_rank_window->set_score(board->GetScore());
//     // if (record_rank_window->score() > 0) {  // 分数>0，才显示排行榜窗口
//     //     record_rank_window->show();
//     //     record_rank_window->setFocus();
//     // }

//     // AudioManager::GetInstance()->PlayClose();
//     // AudioManager::GetInstance()->StopBgm2();
//     // AudioManager::GetInstance()->PlayBgm1();
//     delay(20);
//     // timer_flush_score_and_left_time_bar_->stop();
//     this->close();
// }

void GameWindow::on_skill1_button_clicked() {
    AudioManager::GetInstance()->PlaySkill();
    // BGM::GetInstance()->PlayOpen();
    // board->ClickedOnDiamond();  // 道具1
    // if (board->GetMouseOnDiamond())
    //     ui->skill1_button->setIcon(QIcon(":/images/gamewindow/1-.png"));
    // else
    //     ui->skill1_button->setIcon(QIcon(":/images/gamewindow/1.png"));
}

void GameWindow::on_skill2_button_clicked() {
    AudioManager::GetInstance()->PlaySkill();
    // BGM::GetInstance()->PlayOpen();

    // board->ClickedOnLightning();  // 道具2
    // if (board->GetMouseOnLightning())
    //     ui->skill2_button->setIcon(QIcon(":/images/gamewindow/2-.png"));
    // else
    //     ui->skill2_button->setIcon(QIcon(":/images/gamewindow/2.png"));
}

void GameWindow::on_skill3_button_clicked() {
    AudioManager::GetInstance()->PlaySkill();
    // BGM::GetInstance()->PlayOpen();

    // board->ClickedOnShuffle();  // 道具3
    // if (board->GetMouseOnShuffle())
    //     ui->skill3_button->setIcon(QIcon(":/images/gamewindow/3-.png"));
    // else
    //     ui->skill3_button->setIcon(QIcon(":/images/gamewindow/3.png"));
}

void GameWindow::on_pause_button_clicked() {
    AudioManager::GetInstance()->PauseBgm3();
    PauseWindow *pw = new PauseWindow(this);
    pw->setGeometry(0,0,1280,720);
    pw->show();
    AudioManager::GetInstance()->PauseBgm2();
    connect(pw, &PauseWindow::exitwindow, this, [this]{
        AudioManager::GetInstance()->StopBgm3();
        changeWindow(new MainWindow());
    });
}

void GameWindow::on_hint_button_clicked() {
    //board->ShowHint(true);  // 提示
}

void GameWindow::on_hint_button_pressed() { }

void GameWindow::on_hint_button_released() { }

//void GameWindow::on_btnReturn_pressed() {  }

//void GameWindow::on_btnReturn_released() { }
