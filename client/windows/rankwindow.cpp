#include "rankwindow.h"

#include <QFont>
#include <QFontDatabase>

#include "mainwindow.h"
#include "ui_rankwindow.h"
RankWindow::RankWindow(QWidget *parent) : FrameLessWindow(parent), ui(new Ui::RankWindow) {
    ui->setupUi(this);
    // 固定窗口大小
    this->setFixedSize(1280, 720);
    // 去除自带的边框
    this->setWindowFlag(Qt::FramelessWindowHint);
    // for (int i=0;i<10;i++)
    // Rank::GetInstance()->Insert(QString::number(i).toStdString(),-1);
    // Rank::GetInstance()->Insert("lovekdl", 200);
    //Rank::GetInstance()->Query(pairs);

    // int now = 1;
    // for (const auto &x : pairs) {
    //     if (now == 1) {
    //         ui->label1->setText(QString::fromStdString(x.first));
    //         ui->label2->setText(QString::number(x.second));
    //         if (x.second == -1) {
    //             ui->label1->setText("");
    //             ui->label2->setText("");
    //         }
    //     }
    //     if (now == 2) {
    //         ui->label3->setText(QString::fromStdString(x.first));
    //         ui->label4->setText(QString::number(x.second));
    //         if (x.second == -1) {
    //             ui->label3->setText("");
    //             ui->label4->setText("");
    //         }
    //     }
    //     if (now == 3) {
    //         ui->label5->setText(QString::fromStdString(x.first));
    //         ui->label6->setText(QString::number(x.second));
    //         if (x.second == -1) {
    //             ui->label5->setText("");
    //             ui->label6->setText("");
    //         }
    //     }
    //     if (now == 4) {
    //         ui->label7->setText(QString::fromStdString(x.first));
    //         ui->label8->setText(QString::number(x.second));
    //         if (x.second == -1) {
    //             ui->label7->setText("");
    //             ui->label8->setText("");
    //         }
    //     }
    //     if (now == 5) {
    //         ui->label9->setText(QString::fromStdString(x.first));
    //         ui->label10->setText(QString::number(x.second));
    //         if (x.second == -1) {
    //             ui->label9->setText("");
    //             ui->label10->setText("");
    //         }
    //     }
    //     if (now == 6) {
    //         ui->label11->setText(QString::fromStdString(x.first));
    //         ui->label12->setText(QString::number(x.second));
    //         if (x.second == -1) {
    //             ui->label11->setText("");
    //             ui->label12->setText("");
    //         }
    //     }
    //     if (now == 7) {
    //         ui->label13->setText(QString::fromStdString(x.first));
    //         ui->label14->setText(QString::number(x.second));
    //         if (x.second == -1) {
    //             ui->label13->setText("");
    //             ui->label14->setText("");
    //         }
    //     }
    //     if (now == 8) {
    //         ui->label15->setText(QString::fromStdString(x.first));
    //         ui->label16->setText(QString::number(x.second));
    //         if (x.second == -1) {
    //             ui->label15->setText("");
    //             ui->label16->setText("");
    //         }
    //     }
    //     if (now == 9) {
    //         ui->label17->setText(QString::fromStdString(x.first));
    //         ui->label18->setText(QString::number(x.second));
    //         if (x.second == -1) {
    //             ui->label17->setText("");
    //             ui->label18->setText("");
    //         }
    //     }
    //     if (now == 10) {
    //         ui->label19->setText(QString::fromStdString(x.first));
    //         ui->label20->setText(QString::number(x.second));
    //         if (x.second == -1) {
    //             ui->label19->setText("");
    //             ui->label20->setText("");
    //         }
    //         break;
    //     }
    //     now++;
    // }
    // int font_Id = QFontDatabase::addApplicationFont(":/font/SmileySans-Oblique.ttf");
    // QStringList font_list = QFontDatabase::applicationFontFamilies(font_Id);
    // if (!font_list.isEmpty()) {
    //     QFont f;
    //     f.setFamily(font_list[0]);
    //     f.setPointSize(20);
    //     ui->ID_label->setFont(f);
    //     ui->ScoreLabel->setFont(f);
    //     ui->label1->setFont(f);
    //     ui->label2->setFont(f);
    //     ui->label3->setFont(f);
    //     ui->label4->setFont(f);
    //     ui->label5->setFont(f);
    //     ui->label6->setFont(f);
    //     ui->label7->setFont(f);
    //     ui->label8->setFont(f);
    //     ui->label9->setFont(f);
    //     ui->label10->setFont(f);
    //     ui->label11->setFont(f);
    //     ui->label12->setFont(f);
    //     ui->label13->setFont(f);
    //     ui->label14->setFont(f);
    //     ui->label15->setFont(f);
    //     ui->label16->setFont(f);
    //     ui->label17->setFont(f);
    //     ui->label18->setFont(f);
    //     ui->label19->setFont(f);
    //     ui->label20->setFont(f);
    // }
}

RankWindow::~RankWindow() { delete ui; }

void RankWindow::on_btnReturn_clicked() {
    //BGM::GetInstance()->PlayClose();
    changeWindow(new MainWindow());
}
