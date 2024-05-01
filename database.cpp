#include "database.h"

// 构建数据库
bool db::Database::createDatabase(const QString &dbName) {
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(dbName); // 使用传入的数据库名称参数
    return db_.open();
}

// 查询数据库中是否有tableName表。
bool db::Database::hasTable(const QString &tableName) {
    QStringList tables = db_.tables();
    if (tables.contains(tableName, Qt::CaseInsensitive)) {
        return true;
    }
    else {
        return false;
    }
}

//数据库的初始化。将几个必须用到的表格初始化
bool db::Database::initDataBase() {
    QStringList tables = db_.tables();
    QSqlQuery query(db_);
    // 构建学生表格
    if (!tables.contains("students", Qt::CaseInsensitive) && !query.exec(createStudentTable)) {
        qWarning() << "Failed to create student table:" << query.lastError().text();
        return false;
    }
    // 构建老师表格
    if (!tables.contains("teachers", Qt::CaseInsensitive) && !query.exec(createTeacherTable)) {
        qWarning() << "Failed to create student table:" << query.lastError().text();
        return false;
    }
    if (!tables.contains("wordbank", Qt::CaseInsensitive) && !query.exec(createWordBook)) {
        qWarning() << "Failed to create word book:" << query.lastError().text();
        return false;
    }

    if (!tables.contains("words", Qt::CaseInsensitive) && !query.exec(createWordTable)) {
        qWarning() << "Failed to create word table:" << query.lastError().text();
        return false;
    }

    /*
    if (!tables.contains("class", Qt::CaseInsensitive) && !query.exec(createClassTable)) {
      qWarning() << "Failed to create class table:" << query.lastError().text();
      return false;
    }

    if (!tables.contains("teacherclass", Qt::CaseInsensitive) && !query.exec(createWordTable)) {
      qWarning() << "Failed to create word table:" << query.lastError().text();
      return false;
    }

    if (!tables.contains("studentclass", Qt::CaseInsensitive) && !query.exec(createWordTable)) {
      qWarning() << "Failed to create word table:" << query.lastError().text();
      return false;
    }
*/

    return true;
}





