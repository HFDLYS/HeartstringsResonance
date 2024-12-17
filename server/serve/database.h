#ifndef DATABASE_H
#define DATABASE_H
#include <QVector>
#include <QtSql>
#include <QDebug>
class DataBase
{
private:
    QSqlDatabase db;
    struct Player{//树的节点
        QString userName;
        int pointSolo;
        int pointMulti;
        Player(QString userName_,int pointSolo_,int pointMulti_):
            userName(userName_),pointSolo(pointSolo_),pointMulti(pointMulti_){}
        void setSolo(int a){
            pointSolo=pointSolo<a?a:pointSolo;
        }
        void setMulti(int a){
            pointMulti=pointMulti<a?a:pointMulti;
        }
    };
    bool insert(Player);
    QVector<Player> select(QString);
    bool update(Player);
public:
    DataBase();
    bool update(QString,int,int);
    QVector<QPair<QString,int> > rankSolo(int);
    QVector<QPair<QString,int> > rankMulti(int);
};

#endif // DATABASE_H
