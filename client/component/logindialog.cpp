#include "loginDialog.h"
#include <QMessageBox>
#include <QStyleFactory>
#include <QPainter>
#include <QPixmap>
#include <QWebSocket>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include "../windows/basewindow.h"
const QUrl serverUrl("ws://localhost:1479");
LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("爱の魔法:心海相连");
    setFixedSize(300, 200);



    QLabel *usernameLabel = new QLabel("用户名:", this);
    usernameEdit = new QLineEdit(this);
     usernameEdit->setText("admin");

    QLabel *passwordLabel = new QLabel("密码:", this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
     passwordEdit->setText("1234");

    loginButton = new QPushButton("登录", this);
    registerButton = new QPushButton("注册", this);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(usernameEdit);
    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordEdit);
    mainLayout->addWidget(loginButton);
    mainLayout->addWidget(registerButton);

    setLayout(mainLayout);

    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginDialog::onRegisterClicked);
}

QString LoginDialog::getUsername() const
{
    return usernameEdit->text();
}

QString LoginDialog::getPassword() const
{
    return passwordEdit->text();
}

void LoginDialog::onLoginClicked()
{
    QString username = getUsername();
    QString password = getPassword();
    server=new QWebSocket();
    server->open(serverUrl);
    connect(server,&QWebSocket::connected,this,[=]{
        QJsonObject cmd,parameter;
        cmd["command"]="login";
        parameter["username"]=username;
        parameter["password"]=password;
        cmd["parameter"]=parameter;
        QJsonDocument json(cmd);
        qDebug()<<server->sendBinaryMessage(json.toJson());
    });
    connect(server, &QWebSocket::binaryMessageReceived, this, [=](const QByteArray &message){
        qDebug() << message;
        QJsonDocument jsonIn = QJsonDocument::fromJson(message);
        QJsonObject cmd = jsonIn.object();
        qDebug() << cmd["command"].toString();
        if(cmd["command"].toString() == "login"){
            QJsonObject parameter;
            parameter = cmd["parameter"].toObject();
            bool isSuccess = parameter["isSuccess"].toBool();
            QString info = parameter["info"].toString();
            qDebug() << "12312361827361872" << '\n';
            if(isSuccess){
                QMessageBox::information(this, "登录成功", "欢迎"+username+"来到爱の魔法喵");
                BaseWindow::setPlayer(Player(parameter["player"].toObject()));
                accept();
            } else{
                QMessageBox::information(this, "账号或密码错误喵", info);
            }
        }
    });

}

void LoginDialog::onRegisterClicked()
{
    QString username = getUsername();
    QString password = getPassword();
    server=new QWebSocket();
    server->open(serverUrl);
    connect(server,&QWebSocket::connected,this,[=]{
        QJsonObject cmd,parameter;
        cmd["command"]="register";
        parameter["username"]=username;
        parameter["password"]=password;
        cmd["parameter"]=parameter;
        QJsonDocument json(cmd);
        qDebug()<<server->sendBinaryMessage(json.toJson());
    });
    connect(server, &QWebSocket::binaryMessageReceived, this, [=](const QByteArray &message){
        qDebug() << message;
        QJsonDocument jsonIn = QJsonDocument::fromJson(message);
        QJsonObject cmd = jsonIn.object();
        if(cmd["command"].toString() == "register"){
            QJsonObject parameter;
            parameter = cmd["parameter"].toObject();
            bool isSuccess = parameter["isSuccess"].toBool();
            QString info = parameter["info"].toString();
            if(isSuccess){
                QMessageBox::information(this, "注册成功", username+"成功注册");

            } else{
                QMessageBox::information(this, "注册失败", info);
            }
        }
    });
}

void LoginDialog::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);

    QPainter painter(this);
    QPixmap pixmap(":/images/mainwindow/title.png");

    if (!pixmap.isNull()) {
        QSize widgetSize = size();
        QSize pixmapSize = pixmap.size();
        pixmap = pixmap.scaled(widgetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPoint topLeft((widgetSize.width() - pixmap.width()) / 2,
                       (widgetSize.height() - pixmap.height()) / 2);

        painter.drawPixmap(topLeft, pixmap);
    }
}
