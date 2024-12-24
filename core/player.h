#ifndef PLAYER_H
#define PLAYER_H
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
struct Player {//树的节点
    QString username;
    QString password;
    int pointSolo = 0;
    int pointMulti = 0;
    int skill_1 = 0;
    int skill_2 = 0;
    int skill_3 = 0;
    Player() {};
    Player(QString userName_, QString password_, int pointSolo_ = 0, int pointMulti_ = 0, int skill_1_ = 0, int skill_2_ = 0, int skill_3_ = 0) :
        username(userName_), password(password_), pointSolo(pointSolo_), pointMulti(pointMulti_), skill_1(skill_1_), skill_2(skill_2_), skill_3(skill_3_) {
    }
    Player(QJsonObject in) {
        username = in["username"].toString();
        if (in.contains("password"))password = in["password"].toString();
        if (in.contains("pointSolo"))pointSolo = in["pointSolo"].toInt();
        if (in.contains("pointMulti"))pointMulti = in["pointMulti"].toInt();
        if (in.contains("skill_1"))skill_3 = in["skill_1"].toInt();
        if (in.contains("skill_2"))skill_3 = in["skill_2"].toInt();
        if (in.contains("skill_3"))skill_3 = in["skill_3"].toInt();
    }
    void setSolo(int a) {
        pointSolo = pointSolo < a ? a : pointSolo;
    }
    void setMulti(int a) {
        pointMulti = pointMulti < a ? a : pointMulti;
    }
    void setSkill1(int a) {
        skill_1 = skill_1 < a ? a : skill_1;
    }
    void setSkill2(int a) {
        skill_2 = skill_2 < a ? a : skill_2;
    }
    void setSkill3(int a) {
        skill_3 = skill_3 < a ? a : skill_3;
    }
    QJsonObject toJson() {
        QJsonObject out;
        out["username"] = username;
        out["password"] = password;
        out["pointSolo"] = pointSolo;
        out["pointMulti"] = pointMulti;
        out["skill_1"] = skill_1;
        out["skill_2"] = skill_2;
        out["skill_3"] = skill_3;
        return out;
    }
};
#endif // PLAYER_H
