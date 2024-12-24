#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWebSocket>
#include <QComboBox>
#include <QMessageBox>

#include "basewindow.h"
#include "mainwindow.h"
#include "ui_loginwindow.h"
#include "../utils/picosha2.h"
namespace Ui {
class LoginWindow;
}

class LoginWindow : public BaseWindow {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:

    void onLoginClicked();
    void onRegisterClicked();


private:
    QString getUsername() const;
    QString getPassword() const;
    QString getIp() const;
    QString getPort() const;
    QString getSelectip() const;
    QString getSelectport() const;
    QString getipstring() const;
    void updateIpLineEdit(int);
    void updatePortLineEdit(int);


private:
    Ui::LoginWindow *ui;
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


#endif  // LOGINWINDOW_H
