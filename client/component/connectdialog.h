#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>

class ConnectDialog : public QDialog {
    Q_OBJECT

public:
    ConnectDialog(QWidget *parent = nullptr);
    QString getIp() const;
    QString getPort() const;
    QString getSelectip() const;
    QString getSelectport() const;
    void updateIpLineEdit(int);
    void updatePortLineEdit(int);

private:
    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QComboBox *ipoption;
    QComboBox *portoption;
};

#endif // CONNECTDIALOG_H
