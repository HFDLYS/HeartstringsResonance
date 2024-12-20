#include "ConnectDialog.h"

ConnectDialog::ConnectDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("连接到多人游戏"));

    QLabel *ipLabel = new QLabel(tr("IP 地址:"), this);
    ipLineEdit = new QLineEdit(this);
    ipLineEdit->setText("localhost"); // 默认值

    QLabel *portLabel = new QLabel(tr("端口号:"), this);
    portLineEdit = new QLineEdit(this);
    portLineEdit->setText("1479"); // 默认值

    okButton = new QPushButton(tr("确定"), this);
    cancelButton = new QPushButton(tr("取消"), this);

    //option
    ipoption = new QComboBox(this);
    portoption = new QComboBox(this);
    ipoption->addItem("localhost");
    ipoption->addItem("bjtu.yuki-hana.cn");
    ipoption->addItem("47.116.175.206");
    portoption->addItem("1479");
    // 连接按钮信号
    connect(okButton, &QPushButton::clicked, this, &ConnectDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &ConnectDialog::reject);

    // 监听下拉框选择变化，更新文本框内容
    connect(ipoption, &QComboBox::currentIndexChanged, this, &ConnectDialog::updateIpLineEdit);
    connect(portoption, &QComboBox::currentIndexChanged, this, &ConnectDialog::updatePortLineEdit);


    // 布局设置
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *ipLayout = new QHBoxLayout();
    ipLayout->addWidget(ipLabel);
    ipLayout->addWidget(ipLineEdit);

    QHBoxLayout *portLayout = new QHBoxLayout();
    portLayout->addWidget(portLabel);
    portLayout->addWidget(portLineEdit);

    QHBoxLayout *selectLayout = new QHBoxLayout();
    selectLayout->addWidget(ipoption);
    selectLayout->addWidget(portoption);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(ipLayout);
    mainLayout->addLayout(portLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(selectLayout);

}

QString ConnectDialog::getIp() const {
    return ipLineEdit->text();
}

QString ConnectDialog::getPort() const {
    return portLineEdit->text();
}

QString ConnectDialog::getSelectip() const {
    return ipoption->currentText();
}

QString ConnectDialog::getSelectport() const {
     return portoption->currentText();
}

void ConnectDialog::updateIpLineEdit(int index) {
    ipLineEdit->setText(ipoption->itemText(index));
}

void ConnectDialog::updatePortLineEdit(int index) {
    portLineEdit->setText(portoption->itemText(index));
}
