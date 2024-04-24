#include "role.h"

/*
    qint64 id_;
    QString name_;
    QString password_;
    QString profile_photo_url_;
 */

//====================================== Student part =====================================//
//--------------------------- bind parameters --------------------------//
auto db::Teacher::addTeacher(QSqlQuery &q,
                             const qint64 &id,
                             const QString &name,
                             const QString &password,
                             const QString &profile_photo_url) -> QVariant {
  q.addBindValue(id);
  q.addBindValue(name);
  q.addBindValue(password);
  q.addBindValue(profile_photo_url);
  q.exec();

  return q.lastInsertId();
}

auto db::Teacher::deleteTeacher(QSqlQuery &q, const qint64 &id) -> bool {
  q.addBindValue(id);
  return q.exec();
}

void db::Teacher::displayTeacher(QSqlQuery &q, const qint64 &id) {
  q.addBindValue(id);
  if (!q.exec()) {
    qWarning() << "Query failed:" << q.lastError().text();
  } else {
    qDebug() <<"------ 打印老师信息 -----\n";
    while (q.next()) {
      // 获取每一列的值
      qint64 id = q.value("id").toLongLong();
      QString name = q.value("name").toString();
      QString profilePhotoUrl = q.value("profile_photo_url").toString();

      qDebug() << "id: " << id << "name: " << name << "profilePhotoUrl: " << profilePhotoUrl << "\n";
    }
    qDebug() <<"------ 结束打印老师信息 -----\n";
  }
}

//--------------------------- semantic functions --------------------------//
// 增删改查
auto db::Teacher::registerRole() -> QVariant {
  // 初始化query。
  QSqlQuery query(returnDatabase());
  if(!query.prepare(insertTeacherTable)) {
    throw std::runtime_error("Failed to prepare TearcherTable insert sql");
  }

  return addTeacher(query, GetId(), GetName(), GetPassword(), GetProfilePhotoUrl());
}

auto db::Teacher::cancelRole() ->void {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(deleteTeacherRole)) {
    throw std::runtime_error("Failed to prepare TearcherTable insert sql");
  }

  if(!deleteTeacher(query, GetId())) {
    qWarning() << "Failed to canel Teacher: " << GetId() << "\n";
  }
}

auto db::Teacher::displayInfo() ->void {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(dispalyTeacherRole)) {
    throw std::runtime_error("Failed to prepare TearcherTable insert sql");
  }

  displayTeacher(query, GetId());
}

// 扩展操作
auto db::Teacher::createClass() -> QVariant {
  return false;
}

auto db::Teacher::deleteClass() -> QVariant {
  return false;
}

//====================================== Student part =====================================//
//--------------------------- bind parameters --------------------------//
auto db::Student::addStudent(QSqlQuery &q,
                             const qint64 &id,
                             const QString &name,
                             const QString &password,
                             const QString &profile_photo_url) -> QVariant {
  q.addBindValue(id);
  q.addBindValue(name);
  q.addBindValue(password);
  q.addBindValue(profile_photo_url);
  q.exec();

  return q.lastInsertId();
}

auto db::Student::deleteStudent(QSqlQuery &q, const qint64 &id) -> bool {
  q.addBindValue(id);
  return q.exec();
}

void db::Student::displayStudent(QSqlQuery &q, const qint64 &id) {
  q.addBindValue(id);
  if (!q.exec()) {
    qWarning() << "Query failed:" << q.lastError().text();
  } else {
    qDebug() <<"------ 打印学生信息 -----\n";
    while (q.next()) {
      // 获取每一列的值
      qint64 id = q.value("id").toLongLong();
      QString name = q.value("name").toString();
      QString profilePhotoUrl = q.value("profile_photo_url").toString();

      qDebug() << "id: " << id << "name: " << name << "profilePhotoUrl: " << profilePhotoUrl << "\n";
    }
    qDebug() <<"------ 结束打印学生信息 -----\n";
  }
}

//--------------------------- semantic functions --------------------------//
// 增删改查
auto db::Student::registerRole() -> QVariant {
  // 初始化query。
  QSqlQuery query(returnDatabase());
  if(!query.prepare(insertStudentTable)) {
    throw std::runtime_error("Failed to prepare TearcherTable insert sql");
  }

  return addStudent(query, GetId(), GetName(), GetPassword(), GetProfilePhotoUrl());
}

auto db::Student::cancelRole() ->void {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(deleteStudentRole)) {
    throw std::runtime_error("Failed to prepare TearcherTable insert sql");
  }

  if(!deleteStudent(query, GetId())) {
    qWarning() << "Failed to canel Student: " << GetId() << "\n";
  }
}

auto db::Student::displayInfo() ->void {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(dispalyStduentRole)) {
    throw std::runtime_error("Failed to prepare TearcherTable insert sql");
  }

  displayStudent(query, GetId());
}

// 扩展操作
auto db::Student::joinClass() -> QVariant {

}

auto db::Student::leaveClass() -> QVariant {

}
