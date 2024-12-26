#ifndef PLAYER_H
#define PLAYER_H
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
struct Player {
    QString username;
    QString password;
    int pointSolo = 0;
    int pointMulti = 0;
    int skill_1 = 1;
    int skill_2 = 1;
    int skill_3 = 1;
    int gemStyle = 0;
    int boardStyle = 0;
    int musicStyle = 0;
    int picStyle = 0;
    Player() {};
    Player(QString userName_, QString password_, int pointSolo_ = 0, int pointMulti_ = 0, int skill_1_ = 1, int skill_2_ = 1, int skill_3_ = 1) :
        username(userName_), password(password_), pointSolo(pointSolo_), pointMulti(pointMulti_), skill_1(skill_1_), skill_2(skill_2_), skill_3(skill_3_) {
    }
    Player(QJsonObject in) {
        username = in["username"].toString();
        if (in.contains("password"))password = in["password"].toString();
        if (in.contains("pointSolo"))pointSolo = in["pointSolo"].toInt();
        if (in.contains("pointMulti"))pointMulti = in["pointMulti"].toInt();
        if (in.contains("skill_1"))skill_1 = in["skill_1"].toInt();
        if (in.contains("skill_2"))skill_2 = in["skill_2"].toInt();
        if (in.contains("skill_3"))skill_3 = in["skill_3"].toInt();
        if (in.contains("gemStyle"))gemStyle = in["gemStyle"].toInt();
        if (in.contains("boardStyle"))boardStyle = in["boardStyle"].toInt();
        if (in.contains("musicStyle"))musicStyle = in["musicStyle"].toInt();
        if (in.contains("picStyle"))picStyle = in["picStyle"].toInt();
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
    void setGemStyle(int a) {
        gemStyle = a;
    }
    void setBoardStyle(int a) {
        boardStyle = a;
    }
    void setMusicStyle(int a) {
        musicStyle = a;
    }
    void setPicStyle(int a) {
        picStyle = a;
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
        out["gemStyle"] = gemStyle;
        out["boardStyle"] = boardStyle;
        out["musicStyle"] = musicStyle;
        out["picStyle"] = picStyle;
        return out;
    }
    bool operator== (const Player b){
        return username==b.username;
    }
};
#endif // PLAYER_H
