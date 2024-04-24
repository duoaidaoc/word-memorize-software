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
    QList<qint64> class_ids_;                  // 老师的班级管理，多值属性
    const QLatin1String insertTeacherTable = QLatin1String(R"(
        insert into teachers(id, name, password, profile_photo_url) values(?, ?, ?, ?)
    )");

    // 添加教师语义操作
    static QVariant addTeacher(QSqlQuery &q, const qint64 &id, const QString &name, const QString &password, const QString &profile_photo_url);

public:
    explicit Teacher(Database& db) : Role(db) {}
    QVariant registerRole() override;
    QVariant createClass();
    QVariant deleteClass();
};

class Student : public Role {
private:
    QList<qint64> class_ids_;                  // 学生添加班级。
    const QLatin1String insertStudentTable = QLatin1String(R"(
        insert into students(id, name, password, profile_photo_url) values(?, ?, ?, ?)
    )");

    // 添加学生语义操作
    static QVariant addStudent(QSqlQuery &q, const qint64 &id, const QString &name, const QString &password, const QString &profile_photo_url);

public:
    explicit Student(Database& db) : Role(db) {}
    QVariant registerRole() override;
    QVariant joinClass();
    QVariant leaveClass();
};
} // end db

#endif // ROLE_H

