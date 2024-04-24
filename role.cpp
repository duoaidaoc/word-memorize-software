#include "role.h"

/*
    qint64 id_;
    QString name_;
    QString password_;
    QString profile_photo_url_;
 */

auto db::Teacher::addTeacher(QSqlQuery &q,
                             const qint64 &id,
                             const QString &name,
                             const QString &password,
                             const QString &profile_photo_url) -> QVariant{
  q.addBindValue(id);
  q.addBindValue(name);
  q.addBindValue(password);
  q.addBindValue(profile_photo_url);
  q.exec();

  return q.lastInsertId();
}

auto db::Student::addStudent(QSqlQuery &q,
                             const qint64 &id,
                             const QString &name,
                             const QString &password,
                             const QString &profile_photo_url) -> QVariant{
  q.addBindValue(id);
  q.addBindValue(name);
  q.addBindValue(password);
  q.addBindValue(profile_photo_url);
  q.exec();

  return q.lastInsertId();
}

auto db::Teacher::registerRole() -> QVariant {
  // 初始化query。
  QSqlQuery query(returnDatabase());
  if(!query.prepare(insertTeacherTable)) {
    throw std::runtime_error("Failed to prepare TearcherTable insert sql");
  }

  return addTeacher(query, GetId(), GetName(), GetPassword(), GetProfilePhotoUrl());
}

auto db::Teacher::createClass() -> QVariant {
  return false;
}

auto db::Teacher::deleteClass() -> QVariant {
  return false;
}

auto db::Student::registerRole() -> QVariant {
  // 初始化query。
  QSqlQuery query(returnDatabase());
  if(!query.prepare(insertStudentTable)) {
    throw std::runtime_error("Failed to prepare TearcherTable insert sql");
  }

  return addStudent(query, GetId(), GetName(), GetPassword(), GetProfilePhotoUrl());
}

auto db::Student::joinClass() -> QVariant {

}

auto db::Student::leaveClass() -> QVariant {

}
