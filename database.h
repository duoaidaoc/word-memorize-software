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
        profile_photo_url VARCHAR(255),
        plan BIGINT
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
        primary key(teacher_id, class_id),
        FOREIGN KEY (teacher_id) REFERENCES teachers(id),
        FOREIGN KEY (class_id) REFERENCES class(id)
    )
)");

  const QLatin1String createStudentClassTable = QLatin1String(R"(
    CREATE TABLE IF NOT EXISTS studentclass (
        student_id BIGINT,
        class_id   BIGINT,
        primary key(student_id, class_id),
        FOREIGN KEY (student_id) REFERENCES students(id),
        FOREIGN KEY (class_id) REFERENCES class(id)
    )
)");

  const QLatin1String createTaskTable = QLatin1String(R"(
    CREATE TABLE IF NOT EXISTS tasktable (
        id BIGINT PRIMARY KEY,
        task_name VARCHAR(255),
        create_time DATETIME,
        deadline DATETIME,
        time_limit TIME
    )
)");

  const QLatin1String createAssignmentDistributionTable = QLatin1String(R"(
    CREATE TABLE IF NOT EXISTS AssignmentDistributionTable (
        teacher_id BIGINT,
        task_id BIGINT,
        class_id BIGINT,
        PRIMARY KEY (teacher_id, task_id, class_id),
        FOREIGN KEY (teacher_id) REFERENCES teachers(id),
        FOREIGN KEY (task_id) REFERENCES tasktable(id),
        FOREIGN KEY (class_id) REFERENCES class(id)
    )
)");

  const QLatin1String createTaskWordTable = QLatin1String(R"(
    CREATE TABLE IF NOT EXISTS TaskWordTable (
        task_id BIGINT,
        word_id BIGINT,
        PRIMARY KEY (task_id, word_id),
        FOREIGN KEY (word_id) REFERENCES words(id)
    )
)");

  const QLatin1String createStudentLearnWordBank = QLatin1String(R"(
    CREATE TABLE IF NOT EXISTS createStudentLearnWordBank (
        student_id    BIGINT,
        word_bank_id  BIGINT,
        PRIMARY KEY (student_id),
        FOREIGN KEY (student_id) REFERENCES students(id),
        FOREIGN KEY (word_bank_id) REFERENCES wordbank(id)
    )
)");

  const QLatin1String createStudentWordBank = QLatin1String(R"(
    CREATE TABLE IF NOT EXISTS StudentWordBankTable (
        student_id BIGINT,
        word_id    BIGINT,
        PRIMARY KEY (student_id, word_id),
        FOREIGN KEY (student_id) REFERENCES students(id),
        FOREIGN KEY (word_id) REFERENCES words(id)
    )
)");

  const QLatin1String createStudentWordLearningTable = QLatin1String(R"(
    CREATE TABLE IF NOT EXISTS StudentWordLearning (
        student_id BIGINT,
        word_id BIGINT,
        PRIMARY KEY (student_id, word_id),
        FOREIGN KEY (student_id) REFERENCES students(id),
        FOREIGN KEY (word_id) REFERENCES words(id)
    )
)");

  const QLatin1String createWordWordBank = QLatin1String(R"(
    CREATE TABLE IF NOT EXISTS WordBankRelationTable (
        word_bank_id BIGINT,
        word_id    BIGINT,
        PRIMARY KEY (word_bank_id, word_id),
        FOREIGN KEY (word_bank_id) REFERENCES wordbank(id),
        FOREIGN KEY (word_id) REFERENCES words(id)
    )
)");

  const QLatin1String createStudentSysLearn = QLatin1String(R"(
    CREATE TABLE IF NOT EXISTS StudentSysLearn (
        student_id BIGINT,
        word_id    BIGINT,
        PRIMARY KEY (student_id, word_id),
        FOREIGN KEY (student_id) REFERENCES students(id),
        FOREIGN KEY (word_id) REFERENCES words(id)
    )
)");

  const QLatin1String radioStore = QLatin1String(R"(
    CREATE TABLE IF NOT EXISTS radio (
        name    VARCHAR(255),
        audio   MEDIUMBLOB,
        PRIMARY KEY (name)
    )
)");

  const QLatin1String settingsForTeacher = QLatin1String(R"(
    CREATE TABLE IF NOT EXISTS teacherSettings (
        id      BIGINT,
        pic     MEDIUMBLOB,
        phone   VARCHAR(255),
        age     BIGINT,
        message VARCHAR(255),
        school  VARCHAR(255),
        PRIMARY KEY (id),
        FOREIGN KEY (id) REFERENCES teachers(id)
    )
)");

  const QLatin1String settingsForStudent = QLatin1String(R"(
    CREATE TABLE IF NOT EXISTS studentSettings (
        id      BIGINT,
        pic     MEDIUMBLOB,
        phone   VARCHAR(255),
        age     BIGINT,
        message VARCHAR(255),
        school  VARCHAR(255),
        PRIMARY KEY (id),
        FOREIGN KEY (id) REFERENCES students(id)
    )
)");

  const QLatin1String ranking = QLatin1String(R"(
    CREATE TABLE IF NOT EXISTS rankings (
        id         BIGINT,
        score      BIGINT,
        nickname   VARCHAR(255),
        PRIMARY KEY (id),
        FOREIGN KEY (id) REFERENCES students(id)
    )
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

  // 系统自行生成词库，用来学生直接匹配比赛。
  void generateWordBank() {

  }
};
} // end db

#endif // DATABASE_H
