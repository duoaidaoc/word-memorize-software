#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <stdexcept>

namespace db {
/*
 * 编写database类，封装调用接口
 */
class Database
{
private:
    QSqlDatabase db_;
    bool createDatabase(const QString &dbName);

    // 封装查询表格是否重复的接口。
    bool hasTable(const QString &tableName);
    // 初始化数据库
    bool initDataBase();

    // 构建表格SQL语句
    const QLatin1String createStudentTable = QLatin1String(R"(
        CREATE TABLE IF NOT EXISTS students (
            id BIGINT PRIMARY KEY,
            name VARCHAR(255),
            password VARCHAR(255),
            profile_photo_url VARCHAR(255)
            -- 注意：class_ids_ 不会在这里创建，它应该在一个单独的关联表中创建。
        )
    )");

    const QLatin1String createTeacherTable = QLatin1String(R"(
        CREATE TABLE IF NOT EXISTS teachers (
            id BIGINT PRIMARY KEY,
            name VARCHAR(255),
            password VARCHAR(255),
            profile_photo_url VARCHAR(255)
            -- 注意：class_ids_ 不会在这里创建，它应该在一个单独的关联表中创建。
        )
    )");

    const QLatin1String createWordBook = QLatin1String(R"(
        CREATE TABLE IF NOT EXISTS wordbank (
            id BIGINT PRIMARY KEY,
            name VARCHAR(255),
            picture_url VARCHAR(255)
            -- 注意：word_id_ 不会在这里创建，它应该在一个单独的关联表中创建。
        )
    )");

    const QLatin1String createWordTable = QLatin1String(R"(
        CREATE TABLE IF NOT EXISTS words (
            id BIGINT PRIMARY KEY,
            english   VARCHAR(255),
            chinese   VARCHAR(255),
            phonetic  VARCHAR(255),
            audio_url VARCHAR(255)
        )
    )");

    const QLatin1String insertWordBook = QLatin1String(R"(
        insert into wordbank(id, name, picture_url) values(?, ?, ?)
    )");
    const QLatin1String insertWordTable = QLatin1String(R"(
        insert into words(id, english, chinese, phonetic, audio_url) values(?, ?, ?, ?, ?)
    )");

public:
    Database(const QString &dbName)
    {
        // 创建数据库，并初始化。
        if (!createDatabase(dbName))
            qDebug() << "Failed to open database:" << db_.lastError().text();
        if (!initDataBase())
            qDebug() << "Failed to init database:" << db_.lastError().text();
    }

    ~Database()
    {
        // 关闭数据库连接
        db_.close();
    }

    auto returnDatabase() -> QSqlDatabase& {
        return db_;
    }

    auto isOpen() -> bool {
        return db_.open();
    }
};
} // end db

#endif // DATABASE_H
