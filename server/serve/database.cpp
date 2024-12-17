#include "database.h"

DataBase::DataBase() {
    connect();
}

bool DataBase::connect(){
    db=QSqlDatabase::addDatabase("QMYSQL");
    db.setPort(3306);
    db.setHostName("47.116.175.206");
    db.setUserName("LoM");
    db.setPassword("LoM_10086");
    db.setDatabaseName("LoM_HsR");
    if(db.open())
    {
        qDebug()<<"成功连接数据库";
        return true;
    }
    else {
        qDebug()<<"连接失败,原因:"<<db.lastError().text();
        return false;
    }
}

bool DataBase::insert(Player player){
    if (!db.isOpen()) {
        if (!connect()) {
            return false;
        }
    }
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
    if (!db.isOpen()) {
        if (!connect()) {
            return QVector<DataBase::Player>();
        }
    }
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
    if (!db.isOpen()) {
        if (!connect()) {
            return false;
        }
    }
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
bool DataBase::update(QString username,int pointSolo,int pointMulti){
    if (!db.isOpen()) {
        if (!connect()) {
            return false;
        }
    }
    auto a=select(username);
    if(a.empty()){
        return insert(Player(username,pointSolo,pointMulti));
    }else{
        Player player=a[0];
        player.setSolo(pointSolo);
        player.setMulti(pointMulti);
        return update(player);
    }
}
QVector<QPair<QString,int> > DataBase::rankSolo(int rank){
    if (!db.isOpen()) {
        if (!connect()) {
            return QVector<QPair<QString,int> >();
        }
    }
    QVector<QPair<QString,int> >rec;
    QSqlQuery query(db);
    query.prepare("SELECT user_name,point_solo FROM user ORDER BY point_solo DESC LIMIT :limit;");
    query.bindValue(":limit",rank);
    if(query.exec())
    {
        while (query.next()) {
            rec.push_back(qMakePair(query.record().value("user_name").toString(),query.record().value("point_solo").toInt()));
        }
    }
    return rec;
}
QVector<QPair<QString,int> > DataBase::rankMulti(int rank){
    if (!db.isOpen()) {
        if (!connect()) {
            return QVector<QPair<QString,int> >();
        }
    }
    QVector<QPair<QString,int> >rec;
    QSqlQuery query(db);
    query.prepare("SELECT user_name,point_multi FROM user ORDER BY point_multi DESC LIMIT :limit;");
    query.bindValue(":limit",rank);
    if(query.exec())
    {
        while (query.next()) {
            rec.push_back(qMakePair(query.record().value("user_name").toString(),query.record().value("point_multi").toInt()));
        }
    }
    return rec;
}
