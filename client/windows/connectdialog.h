#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class ConnectDialog : public QDialog {
    Q_OBJECT

public:
    ConnectDialog(QWidget *parent = nullptr);
    QString getIp() const;
    QString getPort() const;

private:
    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // CONNECTDIALOG_H
