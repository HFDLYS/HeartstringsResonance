#include "loginwindow.h"


LoginWindow::LoginWindow(QWidget *parent) : BaseWindow(parent), ui(new Ui::LoginWindow) {
    ui->setupUi(this);
    QLabel *ipLabel = new QLabel(tr("IP 地址:"), this);
    ipLineEdit = new QLineEdit(this);
    ipLineEdit->setText("47.116.175.206"); // 默认值

    QLabel *portLabel = new QLabel(tr("端口号:"), this);
    portLineEdit = new QLineEdit(this);
    portLineEdit->setText("1479"); // 默认值

    ipoption = new QComboBox(this);
    portoption = new QComboBox(this);
    ipoption->addItem("47.116.175.206");
    ipoption->addItem("localhost");
    portoption->addItem("1479");

    QLabel *usernameLabel = new QLabel(tr("用户名:"), this);
    usernameEdit = new QLineEdit(this);

    QLabel *passwordLabel = new QLabel(tr("密码:"), this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("", this);
    registerButton = new QPushButton("", this);

    // 监听下拉框选择变化，更新文本框内容
    auto a=connect(ipoption, &QComboBox::currentIndexChanged, this, &LoginWindow::updateIpLineEdit);
    connections.push_back(a);
    a=connect(portoption, &QComboBox::currentIndexChanged, this, &LoginWindow::updatePortLineEdit);
    connections.push_back(a);

    QHBoxLayout *ipLayout = new QHBoxLayout();
    ipLayout->addWidget(ipLabel);
    ipLayout->addWidget(ipLineEdit);

    QHBoxLayout *portLayout = new QHBoxLayout();
    portLayout->addWidget(portLabel);
    portLayout->addWidget(portLineEdit);

    QHBoxLayout *selectLayout = new QHBoxLayout();
    selectLayout->addWidget(ipoption);
    selectLayout->addWidget(portoption);

    QHBoxLayout *usernameLayout = new QHBoxLayout();
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(usernameEdit);

    QHBoxLayout *passwordLayout = new QHBoxLayout();
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(passwordEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(ipLayout);
    mainLayout->addLayout(portLayout);
    mainLayout->addLayout(selectLayout);
    mainLayout->addLayout(usernameLayout);
    mainLayout->addLayout(passwordLayout);
    loginButton->setFixedHeight(45);
    loginButton->setStyleSheet(
        "QPushButton{border-image: url(:/images/loginwindow/login.png);}"
        "QPushButton:pressed{border-image: url(:/images/loginwindow/login-.png);}"
    );
    registerButton->setFixedHeight(45);
    registerButton->setStyleSheet(
        "QPushButton{border-image: url(:/images/loginwindow/register.png);}"
        "QPushButton:pressed{border-image: url(:/images/loginwindow/register-.png);}"
    );
    mainLayout->addWidget(loginButton);
    mainLayout->addWidget(registerButton);
    ui->loginLayout->addLayout(mainLayout);

    a=connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connections.push_back(a);
    a=connect(registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
    connections.push_back(a);
}

QString LoginWindow::getUsername() const
{
    return usernameEdit->text();
}

QString LoginWindow::getPassword() const
{
    return QString::fromStdString(picosha2::hash256_hex_string(passwordEdit->text().toStdString()));
}

void LoginWindow::onLoginClicked()
{
    loginButton->setEnabled(false);
    registerButton->setEnabled(false);
    QString ip = getIp();
    QString port = getPort();
    QString username = getUsername();
    QString password = getPassword();
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "错误", "用户名或密码不能为空");
        loginButton->setEnabled(true);
        registerButton->setEnabled(true);
        return;
    }
    if (ip.isEmpty() || port.isEmpty()) {
        QMessageBox::warning(this, "错误", "IP地址或端口号不能为空");
        loginButton->setEnabled(true);
        registerButton->setEnabled(true);
        return;
    }
    ipstring = "ws://"+ip+":"+port;
    QUrl userip(ipstring);
    if(server)delete server;
    server=new QWebSocket();
    server->open(userip);
    auto a=connect(server, &QWebSocket::errorOccurred, this,[=]{
        QMessageBox::warning(this, "Error", "链接失败");
        loginButton->setEnabled(true);
        registerButton->setEnabled(true);
    });
    connections.push_back(a);
    a=connect(server,&QWebSocket::connected,this,[=]{
        QJsonObject cmd,parameter;
        cmd["command"]="login";
        parameter["username"]=username;
        parameter["password"]=password;
        cmd["parameter"]=parameter;
        QJsonDocument json(cmd);
        qDebug()<<server->sendBinaryMessage(json.toJson());
    });
    connections.push_back(a);
    a=connect(server, &QWebSocket::binaryMessageReceived, this, [=](const QByteArray &message){
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
                // 加载配置
                GlobalConfig::getInstance().setGemStyle(player.gemStyle);
                GlobalConfig::getInstance().setBoardStyle(player.boardStyle);
                GlobalConfig::getInstance().setMusicStyle(player.musicStyle);
                GlobalConfig::getInstance().setPicStyle(player.picStyle);
                changeWindow(new MainWindow());

            } else{
                QMessageBox::information(this, "账号或密码错误喵", info);
                loginButton->setEnabled(true);
                registerButton->setEnabled(true);
                server->close();
            }
        }
    });
    connections.push_back(a);
}
void LoginWindow::keyPressEvent(QKeyEvent *e) {
    if(e->key()==Qt::Key_Escape){
        QDialog *dialog = new QDialog(this);
        dialog->setFixedSize(400, 170);
        dialog->setWindowTitle("是否退出游戏？");
        //dialog->setWindowFlags(Qt::FramelessWindowHint);
        QHBoxLayout *layout = new QHBoxLayout(dialog);
        QPushButton *confirmButton = new QPushButton("确认", dialog);
        QPushButton *cancelButton = new QPushButton("取消", dialog);
        layout->addWidget(confirmButton);
        layout->addWidget(cancelButton);
        QVBoxLayout *layout2 = new QVBoxLayout(dialog);
        QWidget *tmp = new QWidget(this);
        tmp->setLayout(layout);
        QLabel *label = new QLabel("是否退出游戏?");
        QFont font;
        font.setBold(true);
        font.setPointSize(24);
        font.setFamily("幼圆");
        QFont fontBtn;
        fontBtn.setPointSize(18);
        label->setFont(font);        // 应用新字体
        confirmButton->setFont(fontBtn);
        cancelButton->setFont(fontBtn);
        label->setAlignment(Qt::AlignCenter);
        layout2->addWidget(label);
        layout2->addWidget(tmp);
        dialog->setLayout(layout2);
        auto a=connect(confirmButton, &QPushButton::clicked, dialog, &QDialog::accept);
        connections.push_back(a);
        a=connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);
        if (dialog->exec() == QDialog::Accepted) {
            this->close();
        }
        connections.push_back(a);
    }else if(e->key()==Qt::Key_Enter){
        onLoginClicked();
    }
}
void LoginWindow::onRegisterClicked()
{
    loginButton->setEnabled(false);
    registerButton->setEnabled(false);
    QString ip = getIp();
    QString port = getPort();
    QString username = getUsername();
    QString password = getPassword();
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "用户名或密码不能为空");
        loginButton->setEnabled(true);
        registerButton->setEnabled(true);
        return;
    }
    if (ip.isEmpty() || port.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "IP地址或端口号不能为空");
        loginButton->setEnabled(true);
        registerButton->setEnabled(true);
        return;
    }
    ipstring = "ws://"+ip+":"+port;
    QUrl userip(ipstring);
    if(server)delete server;
    server=new QWebSocket();
    server->open(userip);
    auto a=connect(server, &QWebSocket::errorOccurred, this,[=]{
        QMessageBox::warning(this, "Error", "链接失败");
        loginButton->setEnabled(true);
        registerButton->setEnabled(true);
    });
    connections.push_back(a);
    a=connect(server,&QWebSocket::connected,this,[=]{
        QJsonObject cmd,parameter;
        cmd["command"]="register";
        parameter["username"]=username;
        parameter["password"]=password;
        cmd["parameter"]=parameter;
        QJsonDocument json(cmd);
        qDebug()<<server->sendBinaryMessage(json.toJson());
    });
    connections.push_back(a);
    a=connect(server, &QWebSocket::binaryMessageReceived, this, [=](const QByteArray &message){
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
            } else{
                QMessageBox::information(this, "注册失败", info);
                loginButton->setEnabled(true);
                registerButton->setEnabled(true);
                server->close();
            }
        }
    });
    connections.push_back(a);
}

void LoginWindow::on_btnQuit_clicked() {
    //BGM::GetInstance()->PlayClose();
    QDialog *dialog = new QDialog(this);
    dialog->setFixedSize(400, 170);
    dialog->setWindowTitle("是否退出游戏？");
    //dialog->setWindowFlags(Qt::FramelessWindowHint);
    QHBoxLayout *layout = new QHBoxLayout(dialog);
    QPushButton *confirmButton = new QPushButton("确认", dialog);
    QPushButton *cancelButton = new QPushButton("取消", dialog);
    layout->addWidget(confirmButton);
    layout->addWidget(cancelButton);
    QVBoxLayout *layout2 = new QVBoxLayout(dialog);
    QWidget *tmp = new QWidget(this);
    tmp->setLayout(layout);
    QLabel *label = new QLabel("是否退出游戏?");
    QFont font;
    font.setBold(true);
    font.setPointSize(24);
    font.setFamily("幼圆");
    QFont fontBtn;
    fontBtn.setPointSize(18);
    label->setFont(font);        // 应用新字体
    confirmButton->setFont(fontBtn);
    cancelButton->setFont(fontBtn);
    label->setAlignment(Qt::AlignCenter);
    layout2->addWidget(label);
    layout2->addWidget(tmp);
    dialog->setLayout(layout2);
    auto a=connect(confirmButton, &QPushButton::clicked, dialog, &QDialog::accept);
    connections.push_back(a);
    a=connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);
    if (dialog->exec() == QDialog::Accepted) {
        this->close();
    }
    connections.push_back(a);
}

LoginWindow::~LoginWindow() { delete ui; }

QString LoginWindow::getIp() const {
    return ipLineEdit->text();
}

QString LoginWindow::getPort() const {
    return portLineEdit->text();
}

QString LoginWindow::getSelectip() const {
    return ipoption->currentText();
}

QString LoginWindow::getSelectport() const {
    return portoption->currentText();
}

QString LoginWindow::getipstring() const {
    return ipstring;
}

void LoginWindow::updateIpLineEdit(int index) {
    ipLineEdit->setText(ipoption->itemText(index));
}

void LoginWindow::updatePortLineEdit(int index) {
    portLineEdit->setText(portoption->itemText(index));
}

