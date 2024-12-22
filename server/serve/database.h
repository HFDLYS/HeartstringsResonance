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
        int skill_1;
        int skill_2;
        int skill_3;
        Player(QString userName_,int pointSolo_,int pointMulti_,int skill_1_,int skill_2_,int skill_3_):
            userName(userName_),pointSolo(pointSolo_),pointMulti(pointMulti_),skill_1(skill_1_),skill_2(skill_2_),skill_3(skill_3_){}
        void setSolo(int a){
            pointSolo=pointSolo<a?a:pointSolo;
        }
        void setMulti(int a){
            pointMulti=pointMulti<a?a:pointMulti;
        }
        void setSkill1(int a){
            skill_1=skill_1<a?a:skill_1;
        }
        void setSkill2(int a){
            skill_2=skill_2<a?a:skill_2;
        }
        void setSkill3(int a){
            skill_3=skill_3<a?a:skill_3;
        }
    };
    bool connect();
    bool insert(Player);
    QVector<Player> select(QString);
    bool update(Player);
public:
    DataBase();
    bool update(QString username, int pointSolo,int pointMulti, int skill_1, int skill_2, int skill_3);
    QVector<QPair<QString,int> > rankSolo(int);
    QVector<QPair<QString,int> > rankMulti(int);
};

#endif // DATABASE_H
