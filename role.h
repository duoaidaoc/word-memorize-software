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
    virtual bool registerRole() = 0;

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

public:
    explicit Teacher(Database& db) : Role(db) {}
    bool registerRole() override;
    bool createClass();
    bool deleteClass();
};

class Student : public Role {
private:
    QList<qint64> class_ids_;                  // 学生添加班级。

public:
    explicit Student(Database& db) : Role(db) {}
    bool registerRole() override;
    bool joinClass();
    bool leaveClass();
};
} // end db

#endif // ROLE_H

