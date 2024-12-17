#include "database.h"

DataBase::DataBase() {
    db=QSqlDatabase::addDatabase("QMYSQL");
    db.setPort(3306);
    db.setHostName("47.116.175.206");
    db.setUserName("LoM");
    db.setPassword("LoM_10086");
    db.setDatabaseName("LoM_HsR");
    if(db.open())
    {
        qDebug()<<"成功连接数据库";
    }
    else {
        qDebug()<<"连接失败,原因:"<<db.lastError().text();
    }
}
bool DataBase::insert(Player player){
    QSqlQuery query(db);
    QString sqlStr = "insert into user(user_name,point_solo,point_multi)values(:name,:pointSolo,:pointMulti);";
    query.prepare(sqlStr);
    query.bindValue(":name",player.userName);
    query.bindValue(":pointSolo",player.pointSolo);
    query.bindValue(":pointMulti",player.pointMulti);
    if(query.exec()){
        qDebug()<<"成功插入数据";
        return true;
    }else{
        qDebug()<<"插入数据失败,原因:"<<query.lastError().text();
        return false;
    }
}
QVector<DataBase::Player> DataBase::select(QString username){
    QVector<DataBase::Player>rec;
    QSqlQuery query(db);
    query.prepare("select * from user where user_name=:username order by id asc;");
    query.bindValue(":username",username);
    if(query.exec())
    {
        while (query.next()) {
            DataBase::Player player(query.record().value("user_name").toString(),query.record().value("point_solo").toInt(),query.record().value("point_multi").toInt());
            rec.push_back(player);
        }
    }
    return rec;
}
bool DataBase::update(Player player){
    db.exec("SET NAMES 'UTF8'");
    QSqlQuery query(db);
    query.prepare("update user set point_solo=:pointSolo,point_multi=:pointMulti where user_name=:userName;");
    query.bindValue(":userName",player.userName);
    query.bindValue(":pointMulti",player.pointMulti);
    query.bindValue(":pointSolo",player.pointSolo);
    if(query.exec()){
        qDebug()<<"成功更新数据!";
        return true;
    }else{
        qDebug()<<"更新数据失败,原因:"<<query.lastError().text();
        return false;
    }
}
bool DataBase::updateSolo(QString username,int point){
    auto a=select(username);
    if(a.empty()){
        insert(Player(username,point,0));
    }else{
        Player player=a[0];
        player.setSolo(point);
        update(player);
    }
}
bool DataBase::updateMulti(QString username,int point){
    auto a=select(username);
    if(a.empty()){
        insert(Player(username,0,point));
    }else{
        Player player=a[0];
        player.setMulti(point);
        update(player);
    }
}
