#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWebSocket>
#include <QComboBox>

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    QString getUsername() const;
    QString getPassword() const;
    QString getIp() const;
    QString getPort() const;
    QString getSelectip() const;
    QString getSelectport() const;
    QString getipstring() const;
    void updateIpLineEdit(int);
    void updatePortLineEdit(int);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onLoginClicked();
    void onRegisterClicked();

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QWebSocket *server=nullptr;
    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;
    QComboBox *ipoption;
    QComboBox *portoption;
    QString ipstring;
};

#endif // LOGINDIALOG_H
