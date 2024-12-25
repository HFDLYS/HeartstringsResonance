#include "database.h"

DataBase::DataBase() {
    connect();
}

bool DataBase::connect() {
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setPort(3306);
    db.setHostName("47.116.175.206");
    db.setUserName("LoM");
    db.setPassword("LoM_10086");
    db.setDatabaseName("LoM_HsR");
    db.setConnectOptions("MYSQL_OPT_RECONNECT=1");
    db.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=2");
    if (db.open()) {
        qDebug() << "成功连接数据库";
        return true;
    } else {
        qDebug() << "连接失败,原因:" << db.lastError().text();
        return false;
    }
}

bool DataBase::insert(Player player, QString& info) {
    if (!db.isOpen()) {
        if (!connect()) {
            return false;
        }
    }

    if (select(player.username).size() > 0) {
        info = "用户已存在";
        qDebug() << info;
        return false;
    }


    QSqlQuery query(db);
    QString sqlStr = "insert into user(user_name,password) values(:name,:password);";
    query.prepare(sqlStr);
    query.bindValue(":name", player.username);
    query.bindValue(":password", player.password);
    if (!query.exec()) {
        info = "插入user表失败,原因: " + query.lastError().text();
        qDebug() << info;
        return false;
    }
    // query = QSqlQuery(db);
    int userId = query.lastInsertId().toInt();
    sqlStr = "insert into rank_list(user_id, point_solo, point_multi) values(:userId, :pointSolo, :pointMulti);";
    query.prepare(sqlStr);
    query.bindValue(":userId", userId);
    query.bindValue(":pointSolo", player.pointSolo);
    query.bindValue(":pointMulti", player.pointMulti);
    if (query.exec()) {

    } else {
        info = "插入数据失败,原因:" + query.lastError().text();
        qDebug() << info;
        return false;
    }
    sqlStr = "insert into item(user_id, skill_1, skill_2, skill_3) values(:userId, :skill1, :skill2, :skill3);";
    query.prepare(sqlStr);
    query.bindValue(":userId", userId);
    query.bindValue(":skill1", player.skill_1);
    query.bindValue(":skill2", player.skill_2);
    query.bindValue(":skill3", player.skill_3);
    if (query.exec()) {
        info = "成功插入数据";
        qDebug() << "成功插入数据";
        return true;
    } else {
        info = "插入数据失败,原因:" + query.lastError().text();
        qDebug() << "插入数据失败,原因:" << query.lastError().text();
        return false;
    }
}
QVector<Player> DataBase::select(QString username) {
    if (!db.isOpen()) {
        if (!connect()) {
            return QVector<Player>();
        }
    }
    QVector<Player>rec;
    QSqlQuery query(db);
    query.prepare(R"(
        SELECT 
            user.user_name,
            user.password,
            rank_list.point_solo, 
            rank_list.point_multi, 
            item.skill_1, 
            item.skill_2, 
            item.skill_3
        FROM 
            user
        LEFT JOIN 
            rank_list ON user.user_id = rank_list.user_id
        LEFT JOIN 
            item ON user.user_id = item.user_id
        WHERE 
            user.user_name = :username;
    )");
    query.bindValue(":username", username);
    if (query.exec()) {
        qDebug() << "查询成功,有" << query.size() << "条记录";
        while (query.next()) {
            Player player(query.record().value("user_name").toString(), query.record().value("password").toString(), query.record().value("point_solo").toInt(), query.record().value("point_multi").toInt(), query.record().value("skill_1").toInt(), query.record().value("skill_2").toInt(), query.record().value("skill_3").toInt());
            rec.push_back(player);
        }
    } else {
        QString info = "查询失败:" + query.lastError().text();
        qDebug() << info;
    }
    return rec;
}
bool DataBase::update(Player player) {
    auto rec=update(player.username, player.pointSolo, player.pointMulti, player.skill_1, player.skill_2, player.skill_3);
    return !rec.empty();
}

QPair<bool, QString> DataBase::playerRegister(QString username, QString password) {
    QString info;
    return qMakePair(insert(Player(username, password), info), info);
}

QPair<bool, QString> DataBase::playerLogIn(QString username, QString password, Player& player) {
    auto rec = select(username);
    if (rec.empty()) {
        return qMakePair(0, "用户不存在.");
    } else if (rec[0].password != password) {
        return qMakePair(0, "用户名或密码错误.");
    }
    player = rec[0];
    return qMakePair(1, "登录成功");
}

QVector<Player> DataBase::update(QString username, int pointSolo, int pointMulti, int skill_1, int skill_2, int skill_3) {
    QVector<Player> rec;
    if (!db.isOpen()) {
        if (!connect()) {
            return rec;
        }
    }

    auto a = select(username);
    if (a.empty()) {
        //游戏前必然登录
        // 如果用户不存在，执行插入操作
        return rec;//insert(Player(username, pointSolo, pointMulti, skill_1, skill_2, skill_3)); // insert 为插入操作
    } else {
        // 如果用户存在，执行更新操作
        Player player = a[0]; // 假设 a[0] 是已找到的用户
        player.setSolo(pointSolo);
        player.setMulti(pointMulti);
        player.setSkill1(skill_1);
        player.setSkill2(skill_2);
        player.setSkill3(skill_3);

        // 更新数据库中用户的信息
        QSqlQuery query;
        query.prepare("UPDATE rank_list "
            "SET point_solo = :pointSolo, point_multi = :pointMulti "
            "WHERE user_id = (SELECT user_id FROM user WHERE user_name = :username)");
        query.bindValue(":pointSolo", player.pointSolo);
        query.bindValue(":pointMulti", player.pointMulti);
        query.bindValue(":username", player.username);

        if (!query.exec()) {
            qDebug() << "Failed to update rank table:" << query.lastError().text();
            return rec;
        }

        query.prepare("UPDATE item "
            "SET skill_1 = :skill_1, skill_2 = :skill_2, skill_3 = :skill_3 "
            "WHERE user_id = (SELECT user_id FROM user WHERE user_name = :username)");
        query.bindValue(":skill_1", player.skill_1);
        query.bindValue(":skill_2", player.skill_2);
        query.bindValue(":skill_3", player.skill_3);
        query.bindValue(":username", player.username);

        if (!query.exec()) {
            qDebug() << "Failed to update item table:" << query.lastError().text();
            return rec;
        }
        qDebug() << "Update successfully";
        rec=select(username);
        return rec;
    }
}

QVector<QPair<QString, int> > DataBase::rankSolo(int rank) {
    if (!db.isOpen()) {
        if (!connect()) {
            return QVector<QPair<QString, int> >();
        }
    }
    QVector<QPair<QString, int> >rec;
    QSqlQuery query(db);
    query.prepare(R"(
    SELECT user_name,point_solo
        FROM user
        LEFT JOIN rank_list ON user.user_id = rank_list.user_id
        ORDER BY point_solo DESC LIMIT :limit;
    )");
    query.bindValue(":limit", rank);
    if (query.exec()) {
        while (query.next()) {
            rec.push_back(qMakePair(query.record().value("user_name").toString(), query.record().value("point_solo").toInt()));
        }
    }
    return rec;
}
QVector<QPair<QString, int> > DataBase::rankMulti(int rank) {
    if (!db.isOpen()) {
        if (!connect()) {
            return QVector<QPair<QString, int> >();
        }
    }
    QVector<QPair<QString, int> >rec;
    QSqlQuery query(db);
    query.prepare(R"(
    SELECT user_name,point_multi
        FROM user
        LEFT JOIN rank_list ON user.user_id = rank_list.user_id
        ORDER BY point_multi DESC LIMIT :limit;
    )");
    query.bindValue(":limit", rank);
    if (query.exec()) {
        while (query.next()) {
            rec.push_back(qMakePair(query.record().value("user_name").toString(), query.record().value("point_multi").toInt()));
        }
    }
    return rec;
}
