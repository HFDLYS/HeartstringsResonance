#include "ConnectDialog.h"

ConnectDialog::ConnectDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("连接到多人游戏"));

    QLabel *ipLabel = new QLabel(tr("IP 地址:"), this);
    ipLineEdit = new QLineEdit(this);
    ipLineEdit->setText("bjtu.yuki-hana.cn"); // 默认值

    QLabel *portLabel = new QLabel(tr("端口号:"), this);
    portLineEdit = new QLineEdit(this);
    portLineEdit->setText("1479"); // 默认值

    okButton = new QPushButton(tr("确定"), this);
    cancelButton = new QPushButton(tr("取消"), this);

    // 连接按钮信号
    connect(okButton, &QPushButton::clicked, this, &ConnectDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &ConnectDialog::reject);

    // 布局设置
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *ipLayout = new QHBoxLayout();
    ipLayout->addWidget(ipLabel);
    ipLayout->addWidget(ipLineEdit);

    QHBoxLayout *portLayout = new QHBoxLayout();
    portLayout->addWidget(portLabel);
    portLayout->addWidget(portLineEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(ipLayout);
    mainLayout->addLayout(portLayout);
    mainLayout->addLayout(buttonLayout);
}

QString ConnectDialog::getIp() const {
    return ipLineEdit->text();
}

QString ConnectDialog::getPort() const {
    return portLineEdit->text();
}
