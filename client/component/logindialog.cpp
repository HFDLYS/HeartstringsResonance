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
LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("爱の魔法:心海相连");
    setFixedSize(300, 300);

    QLabel *ipLabel = new QLabel(tr("IP 地址:"), this);
    ipLineEdit = new QLineEdit(this);
    ipLineEdit->setText("localhost"); // 默认值

    QLabel *portLabel = new QLabel(tr("端口号:"), this);
    portLineEdit = new QLineEdit(this);
    portLineEdit->setText("1479"); // 默认值

    ipoption = new QComboBox(this);
    portoption = new QComboBox(this);
    ipoption->addItem("localhost");
    ipoption->addItem("nas.yuki-hana.cn");
    ipoption->addItem("bjtu.yuki-hana.cn");
    ipoption->addItem("47.116.175.206");
    portoption->addItem("1479");

    QLabel *usernameLabel = new QLabel("用户名:", this);
    usernameEdit = new QLineEdit(this);

    QLabel *passwordLabel = new QLabel("密码:", this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("登录", this);
    registerButton = new QPushButton("注册", this);

    // 监听下拉框选择变化，更新文本框内容
    connect(ipoption, &QComboBox::currentIndexChanged, this, &LoginDialog::updateIpLineEdit);
    connect(portoption, &QComboBox::currentIndexChanged, this, &LoginDialog::updatePortLineEdit);

    QHBoxLayout *ipLayout = new QHBoxLayout();
    ipLayout->addWidget(ipLabel);
    ipLayout->addWidget(ipLineEdit);

    QHBoxLayout *portLayout = new QHBoxLayout();
    portLayout->addWidget(portLabel);
    portLayout->addWidget(portLineEdit);

    QHBoxLayout *selectLayout = new QHBoxLayout();
    selectLayout->addWidget(ipoption);
    selectLayout->addWidget(portoption);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(ipLayout);
    mainLayout->addLayout(portLayout);
    mainLayout->addLayout(selectLayout);
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
    loginButton->setEnabled(false);
    registerButton->setEnabled(false);
    QString ip = getIp();
    QString port = getPort();
    QString username = getUsername();
    QString password = getPassword();
    ipstring = "ws://"+ip+":"+port;
    QUrl userip(ipstring);
    server=new QWebSocket();
    server->open(userip);
    connect(server, &QWebSocket::errorOccurred, this,[=]{
        QMessageBox::warning(this, "Error", "链接失败");
        loginButton->setEnabled(true);
        registerButton->setEnabled(true);
    });
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
            if(isSuccess){
                QMessageBox::information(this, "登录成功", "欢迎"+username+"来到爱の魔法喵");
                BaseWindow::setPlayer(Player(parameter["player"].toObject()));
                BaseWindow::setip(getipstring());
                BaseWindow::setserver(server);
                loginButton->setEnabled(true);
                registerButton->setEnabled(true);
                accept();

            } else{
                QMessageBox::information(this, "账号或密码错误喵", info);
                loginButton->setEnabled(true);
                registerButton->setEnabled(true);
                server->close();
                delete server;
            }
        }
    });

}

void LoginDialog::onRegisterClicked()
{
    loginButton->setEnabled(false);
    registerButton->setEnabled(false);
    QString ip = getIp();
    QString port = getPort();
    QString username = getUsername();
    QString password = getPassword();
    ipstring = "ws://"+ip+":"+port;
    QUrl userip(ipstring);
    server=new QWebSocket();
    server->open(userip);
    connect(server, &QWebSocket::errorOccurred, this,[=]{
        QMessageBox::warning(this, "Error", "链接失败");
        loginButton->setEnabled(true);
        registerButton->setEnabled(true);
    });
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
                loginButton->setEnabled(true);
                registerButton->setEnabled(true);
                server->close();
                delete server;
            } else{
                QMessageBox::information(this, "注册失败", info);
                loginButton->setEnabled(true);
                registerButton->setEnabled(true);
                server->close();
                delete server;
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


QString LoginDialog::getIp() const {
    return ipLineEdit->text();
}

QString LoginDialog::getPort() const {
    return portLineEdit->text();
}

QString LoginDialog::getSelectip() const {
    return ipoption->currentText();
}

QString LoginDialog::getSelectport() const {
    return portoption->currentText();
}

QString LoginDialog::getipstring() const {
    return ipstring;
}

void LoginDialog::updateIpLineEdit(int index) {
    ipLineEdit->setText(ipoption->itemText(index));
}

void LoginDialog::updatePortLineEdit(int index) {
    portLineEdit->setText(portoption->itemText(index));
}

