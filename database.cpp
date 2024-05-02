#include "database.h"

// 构建数据库
bool db::Database::createDatabase(const QString &dbName) {
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(dbName); // 使用传入的数据库名称参数
    if (!db_.open()) {
      qDebug() << "Error: connection with database fail";
      return false;
    } else {
      qDebug() << "Database: connection ok";
      QSqlQuery query(db_);
      if (!query.exec("PRAGMA foreign_keys = ON")) {
        qDebug() << "Error: failed to enable foreign keys support";
      } else {
        qDebug() << "Foreign keys support enabled";
      }

      return true;
    }
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
    // 创建单词表格
    if (!tables.contains("wordbank", Qt::CaseInsensitive) && !query.exec(createWordBook)) {
        qWarning() << "Failed to create word book:" << query.lastError().text();
        return false;
    }

    if (!tables.contains("words", Qt::CaseInsensitive) && !query.exec(createWordTable)) {
        qWarning() << "Failed to create word table:" << query.lastError().text();
        return false;
    }
    // 创建班级表格
    if (!tables.contains("class", Qt::CaseInsensitive) && !query.exec(createClassTable)) {
      qWarning() << "Failed to create class table:" << query.lastError().text();
      return false;
    }
    // 创建关系表格
    if (!tables.contains("teacherclass", Qt::CaseInsensitive) && !query.exec(createTeacherClassTable)) {
      qWarning() << "Failed to create teacher class table:" << query.lastError().text();
      return false;
    }

    if (!tables.contains("studentclass", Qt::CaseInsensitive) && !query.exec(createStudentClassTable)) {
      qWarning() << "Failed to create student class table:" << query.lastError().text();
      return false;
    }

    if (!tables.contains("tasktable", Qt::CaseInsensitive) && !query.exec(createTaskTable)) {
      qWarning() << "Failed to create task table:" << query.lastError().text();
      return false;
    }

    // Attempt to create AssignmentDistributionTable
    if (!tables.contains("AssignmentDistributionTable", Qt::CaseInsensitive) && !query.exec(createAssignmentDistributionTable)) {
      qWarning() << "Failed to create Assignment Distribution Table:" << query.lastError().text();
      return false;
    }

    // Attempt to create TaskWordTable
    if (!tables.contains("TaskWordTable", Qt::CaseInsensitive) && !query.exec(createTaskWordTable)) {
      qWarning() << "Failed to create Task Word Table:" << query.lastError().text();
      return false;
    }

    // Attempt to create StudentWordBankTable
    if (!tables.contains("StudentWordBankTable", Qt::CaseInsensitive) && !query.exec(createStudentWordBank)) {
      qWarning() << "Failed to create Student Word Bank Table:" << query.lastError().text();
      return false;
    }

    // Attempt to create StudentTaskProgress
    if (!tables.contains("StudentTaskProgress", Qt::CaseInsensitive) && !query.exec(createStudentTaskProgressTable)) {
      qWarning() << "Failed to create Student Task Progress Table:" << query.lastError().text();
      return false;
    }

    // Attempt to create StudentWordLearning
    if (!tables.contains("StudentWordLearning", Qt::CaseInsensitive) && !query.exec(createStudentWordLearningTable)) {
      qWarning() << "Failed to create Student Word Learning Table:" << query.lastError().text();
      return false;
    }

    // Attempt to create WordBankRelationTable
    if (!tables.contains("WordBankRelationTable", Qt::CaseInsensitive) && !query.exec(createWordWordBank)) {
      qWarning() << "Failed to create Word Bank Relation Table:" << query.lastError().text();
      return false;
    }

    return true;
}





