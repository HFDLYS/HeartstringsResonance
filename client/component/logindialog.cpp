#include "loginDialog.h"
#include <QMessageBox>
#include <QStyleFactory>
#include <QPainter>
#include <QPixmap>
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

    if (username == "admin" && password == "1234") {
        QMessageBox::information(this, "登录成功", "欢迎回来，" + username);
        accept();
    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误");
    }
}

void LoginDialog::onRegisterClicked()
{
    QString username = getUsername();
    QString password = getPassword();
    QMessageBox::information(this, "注册成功", username+"注册成功");
    //缺少逻辑：数据库已经存在用户名
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
