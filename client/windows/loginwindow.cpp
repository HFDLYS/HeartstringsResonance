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
    ipoption->addItem("nas.yuki-hana.cn");
    ipoption->addItem("bjtu.yuki-hana.cn");
    portoption->addItem("1479");

    QLabel *usernameLabel = new QLabel(tr("用户名:"), this);
    usernameEdit = new QLineEdit(this);

    QLabel *passwordLabel = new QLabel(tr("密码:"), this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("登录", this);
    registerButton = new QPushButton("注册", this);

    // 监听下拉框选择变化，更新文本框内容
    connect(ipoption, &QComboBox::currentIndexChanged, this, &LoginWindow::updateIpLineEdit);
    connect(portoption, &QComboBox::currentIndexChanged, this, &LoginWindow::updatePortLineEdit);

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
    loginButton->setFixedHeight(40);
    mainLayout->addWidget(loginButton);
    registerButton->setFixedHeight(40);
    mainLayout->addWidget(registerButton);

    ui->loginLayout->addLayout(mainLayout);

    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
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
    ipstring = "ws://"+ip+":"+port;
    QUrl userip(ipstring);
    if(server)delete server;
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
                changeWindow(new MainWindow());

            } else{
                QMessageBox::information(this, "账号或密码错误喵", info);
                loginButton->setEnabled(true);
                registerButton->setEnabled(true);
                server->close();
            }
        }
    });

}

void LoginWindow::onRegisterClicked()
{
    loginButton->setEnabled(false);
    registerButton->setEnabled(false);
    QString ip = getIp();
    QString port = getPort();
    QString username = getUsername();
    QString password = getPassword();
    ipstring = "ws://"+ip+":"+port;
    QUrl userip(ipstring);
    if(server)delete server;
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
            } else{
                QMessageBox::information(this, "注册失败", info);
                loginButton->setEnabled(true);
                registerButton->setEnabled(true);
                server->close();
            }
        }
    });
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

