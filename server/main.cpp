#include <QApplication>
#include <ctime>
#include <iostream>
#include <random>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include "./windows/mainwindow.h"

int main(int argc, char *argv[]) {
    srand(time(0));
    QApplication a(argc, argv);
    qDebug() << QSqlDatabase::drivers();
    MainWindow m;
    m.show();
    return a.exec();
}
