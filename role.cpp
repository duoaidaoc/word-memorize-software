#include "role.h"
#include "class.h"

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

auto db::Teacher::addTeacherClass(QSqlQuery &q,
                             const qint64 &teacher_id,
                             const qint64 &class_id) -> QVariant {
  q.addBindValue(teacher_id);
  q.addBindValue(class_id);
  q.exec();

  return q.lastInsertId();
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
// @todo
auto db::Teacher::createClass(const qint64 &class_id, const QString &class_name, const QString &class_cue) -> QVariant {
  Class class_(returnDB());
  class_.SetId(class_id);
  class_.SetName(class_name);
  class_.SetCue(class_cue);

  // @todo 在老师class表格中插入条目。
  QSqlQuery query(returnDatabase());
  if(!query.prepare(insertTeacherClassTable)) {
    throw std::runtime_error("Failed to prepare TearcherClassTable insert sql");
  }
  addTeacherClass(query, GetId(), class_id);

  // 将创建的班级在班级表中插入。
  return class_.registerClass();
}

// @todo
auto db::Teacher::deleteClass(const qint64 &class_id) -> QVariant {
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

auto db::Student::addStudentClass(QSqlQuery &q,
                                  const qint64 &student_id,
                                  const qint64 &class_id) -> QVariant {
  q.addBindValue(student_id);
  q.addBindValue(class_id);
  if (!q.exec()) {
    qDebug() << "Error inserting data:" << q.lastError().text();
    return QVariant(); // Return an empty QVariant or handle the error as needed
  }
  return q.lastInsertId();
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

auto db::Student::deleteStudentClass(QSqlQuery &q, const qint64 &student_id, const qint64 &class_id) -> bool {
  q.addBindValue(student_id);
  q.addBindValue(class_id);
  if (!q.exec()) {
    qDebug() << "Delete operation failed:" << q.lastError().text();
    return false;  // 删除失败返回false
  }
  else {
    return true;
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
// 学生加入班级，只需要单纯的将studnet_id和class_id插入表格即可。
auto db::Student::joinClass(const qint64 &class_id) -> QVariant {
  // @todo 在老师class表格中插入条目。
  QSqlQuery query(returnDatabase());
  if(!query.prepare(insertStudentClassTable)) {
    throw std::runtime_error("Failed to prepare TearcherClassTable insert sql");
  }
  return addStudentClass(query, GetId(), class_id);
}

auto db::Student::leaveClass(const qint64 &class_id) -> QVariant {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(deleteStudentFromClass)) {
    throw std::runtime_error("Failed to prepare TearcherTable insert sql");
  }
  if(!deleteStudentClass(query, GetId(), class_id)) {
    qWarning() << "Student failed to leave class: " << GetId() << "\n";
  }
}
