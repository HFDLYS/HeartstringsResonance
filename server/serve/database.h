#ifndef DATABASE_H
#define DATABASE_H
#include <QVector>
#include <QtSql>
#include <QDebug>
#include "../../core/player.h"
class DataBase
{
private:
    QSqlDatabase db;
    bool connect();
    bool insert(Player,QString&);
    QVector<Player> select(QString);
    bool update(Player);
public:
    DataBase();
    QPair<bool,QString> playerRegister(QString,QString);
    QPair<bool,QString> playerLogIn(QString username,QString password,Player&player);
    bool update(QString username, int pointSolo,int pointMulti, int skill_1, int skill_2, int skill_3);
    QVector<QPair<QString,int> > rankSolo(int);
    QVector<QPair<QString,int> > rankMulti(int);
};

#endif // DATABASE_H
