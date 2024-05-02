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
        )
    )");

    const QLatin1String createTeacherTable = QLatin1String(R"(
        CREATE TABLE IF NOT EXISTS teachers (
            id BIGINT PRIMARY KEY,
            name VARCHAR(255),
            password VARCHAR(255),
            profile_photo_url VARCHAR(255)
        )
    )");

    const QLatin1String createWordBook = QLatin1String(R"(
        CREATE TABLE IF NOT EXISTS wordbank (
            id BIGINT PRIMARY KEY,
            name VARCHAR(255),
            picture_url VARCHAR(255)
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

    const QLatin1String createClassTable = QLatin1String(R"(
        CREATE TABLE IF NOT EXISTS class (
            id BIGINT PRIMARY KEY,
            name      VARCHAR(255),
            cue       VARCHAR(255)
        )
    )");

    const QLatin1String createTeacherClassTable = QLatin1String(R"(
        CREATE TABLE IF NOT EXISTS teacherclass (
            teacher_id BIGINT,
            class_id   BIGINT,
            primary key(teacher_id, class_id)
            FOREIGN KEY(class_id) REFERENCES class(id) ON DELETE CASCADE
        )
    )");

    const QLatin1String createStudentClassTable = QLatin1String(R"(
        CREATE TABLE IF NOT EXISTS studentclass (
            student_id BIGINT,
            class_id   BIGINT,
            primary key(student_id, class_id)
            FOREIGN KEY(class_id) REFERENCES class(id) ON DELETE CASCADE
        )
    )");

    const QLatin1String createTaskTable = QLatin1String(R"(
        CREATE TABLE IF NOT EXISTS tasktable (
            id BIGINT PRIMARY KEY,
            create_time DATETIME,
            deadline DATETIME,
            time_limit TIME
        )
    )");

    // 老师布置任务
    const QLatin1String createAssignmentDistributionTable = QLatin1String(R"(
        CREATE TABLE IF NOT EXISTS AssignmentDistributionTable (
            teacher_id BIGINT,
            task_id BIGINT,
            class_id BIGINT,
            PRIMARY KEY (teacher_id, task_id, class_id)
        )
    )");

    // 任务单词关系
    const QLatin1String createTaskWordTable = QLatin1String(R"(
        CREATE TABLE IF NOT EXISTS TaskWordTable (
            word_id BIGINT,
            task_id BIGINT,
            PRIMARY KEY (word_id, task_id)
        )
    )");

    // 学生学习词库
    const QLatin1String createStudentWordBank = QLatin1String(R"(
        CREATE TABLE IF NOT EXISTS StudentWordBankTable (
            student_id BIGINT,
            word_id    BIGINT,
            PRIMARY KEY (student_id, word_id)
        )
    )");

    // 学生任务进度表格
    const QLatin1String createStudentTaskProgressTable = QLatin1String(R"(
        CREATE TABLE IF NOT EXISTS StudentTaskProgress (
            student_id BIGINT,
            task_id BIGINT,
            start_time TIME,
            learned_tag BOOLEAN,
            PRIMARY KEY (student_id, task_id)
        )
    )");

    // 学生单词进度表格
    const QLatin1String createStudentWordLearningTable = QLatin1String(R"(
        CREATE TABLE IF NOT EXISTS StudentWordLearning (
            student_id BIGINT,
            word_id BIGINT,
            learned_tag BOOLEAN,
            PRIMARY KEY (student_id, word_id)
        )
    )");

    // 词库单词关系表格
    const QLatin1String createWordWordBank = QLatin1String(R"(
        CREATE TABLE IF NOT EXISTS WordBankRelationTable (
            word_bank_id BIGINT,
            word_id    BIGINT,
            PRIMARY KEY (word_bank_id, word_id)
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
