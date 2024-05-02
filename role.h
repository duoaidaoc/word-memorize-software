#ifndef ROLE_H
#define ROLE_H

#include "table.h"
#include <QString>
#include <QVariant>

namespace db {
class Role : public Table {
private:
    qint64 id_;
    QString name_;
    QString password_;
    QString profile_photo_url_;

public:
    explicit Role(Database& db) : Table(db) {}
    virtual QVariant registerRole() = 0;
    virtual void cancelRole() = 0;
    virtual void displayInfo() = 0;

    void SetId(const qint64& id) { id_ = id; }
    void SetName(const QString& name) { name_ = name; }
    void SetPassword(const QString& password) { password_ = password; }
    void SetProfilePhotoUrl(const QString& url) { profile_photo_url_ = url; }

    const qint64 GetId() const { return id_; }
    const QString GetName() const { return name_; }
    const QString GetPassword() const { return password_; }
    const QString GetProfilePhotoUrl() const { return profile_photo_url_; }
};

class Teacher : public Role {

private:
    const QLatin1String insertTeacherTable = QLatin1String(R"(
        insert into teachers(id, name, password, profile_photo_url) values(?, ?, ?, ?)
    )");
    const QLatin1String deleteTeacherRole = QLatin1String(R"(
        DELETE FROM teachers WHERE id = ?
    )");
    const QLatin1String dispalyTeacherRole = QLatin1String(R"(
        SELECT * FROM teachers WHERE id = ?
    )");
    const QLatin1String insertTeacherClassTable = QLatin1String(R"(
        insert into teacherclass(teacher_id, class_id) values(?, ?)
    )");

    const QLatin1String teacherDeleteClass = QLatin1String(R"(
        DELETE FROM class WHERE id = ?
    )");

    const QLatin1String teacherDeleteTeacherClass = QLatin1String(R"(
        DELETE FROM teacherclass WHERE teacher_id = ? AND class_id = ?
    )");

    const QLatin1String teacherDeleteStudentClass = QLatin1String(R"(
        DELETE FROM studentclass WHERE class_id = ?
    )");

    // 教师语义操作
    static QVariant addTeacher(QSqlQuery &q, const qint64 &id, const QString &name, const QString &password, const QString &profile_photo_url);
    static bool deleteTeacher(QSqlQuery &q, const qint64 &id);
    static void displayTeacher(QSqlQuery &q, const qint64 &id);
    static QVariant addTeacherClass(QSqlQuery &q, const qint64 &teacher_id, const qint64 &class_id);
    static bool teacherDeleteClassTable(QSqlQuery &q, const qint64 &class_id);
    static int teacherDeleteTeacherClassTable(QSqlQuery &q, const qint64 &teacher_id, const qint64 &class_id);
    static bool teacherDeleteStudentClassTable(QSqlQuery &q, const qint64 &class_id);

public:
    explicit Teacher(Database& db) : Role(db) {}

    // 增删改查
    QVariant registerRole() override;
    void cancelRole() override;
    void displayInfo() override;

    // 扩展操作
    // 老师创建班级需要：Class_id, Class_name, Class_cue
    QVariant createClass(const qint64 &class_id, const QString &class_name, const QString &class_cue);
    // 老师删除班级只需要：Class_id
    bool deleteClass(const qint64 &class_id);
};

class Student : public Role {
private:
    const QLatin1String insertStudentTable = QLatin1String(R"(
        insert into students(id, name, password, profile_photo_url) values(?, ?, ?, ?)
    )");
    const QLatin1String deleteStudentRole = QLatin1String(R"(
        DELETE FROM students WHERE id = ?
    )");
    const QLatin1String dispalyStduentRole = QLatin1String(R"(
        SELECT * FROM students WHERE id = ?
    )");
    const QLatin1String insertStudentClassTable = QLatin1String(R"(
        insert into studentclass(student_id, class_id) values(?, ?)
    )");
    const QLatin1String deleteStudentFromClass = QLatin1String(R"(
        DELETE FROM studentclass WHERE student_id = ? AND class_id = ?
    )");

    // 学生语义操作
    static QVariant addStudent(QSqlQuery &q, const qint64 &id, const QString &name, const QString &password, const QString &profile_photo_url);
    static bool deleteStudent(QSqlQuery &q, const qint64 &id);
    static void displayStudent(QSqlQuery &q, const qint64 &id);
    static QVariant addStudentClass(QSqlQuery &q, const qint64 &student_id, const qint64 &class_id);
    static bool deleteStudentClass(QSqlQuery &q, const qint64 &student_id, const qint64 &class_id);

public:
    explicit Student(Database& db) : Role(db) {}

    // 增删改查
    QVariant registerRole() override;
    void cancelRole() override;
    void displayInfo() override;

    // 扩展操作
    QVariant joinClass(const qint64 &class_id);
    bool leaveClass(const qint64 &class_id);
};
} // end db

#endif // ROLE_H

