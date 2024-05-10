#ifndef TASK_H
#define TASK_H

#include "table.h"
#include <QString>
#include <QVariant>

namespace db {
class Task : Table{
private:
  qint64 task_id_;
  QDateTime create_time_;
  QDateTime deadline_;
  QTime time_;
  QString task_name_;

  const QLatin1String insertTaskTable = QLatin1String(R"(
        insert into tasktable(id, task_name, create_time, deadline, time_limit) values(?, ?, ?, ?, ?)
    )");

  static QVariant addTask(QSqlQuery &q, const qint64 &task_id, const QString& task_name, const QDateTime& create_time, const QDateTime& deadline, const QTime& time);

public:
  explicit Task(Database& db) : Table(db) {}
  QVariant registerTask();

  void SetId(const qint64& id) { task_id_ = id; }
  void SetCreateTime(const QDateTime& create_time) { create_time_ = create_time; }
  void SetDeadline(const QDateTime& deadline) { deadline_ = deadline; }
  void SetTime(const QTime& time) { time_ = time; }
  void SetTaskName(const QString &task_name) { task_name_ = task_name; }

  const qint64 GetId() const { return task_id_; }
  const QDateTime GetCreateTime() const { return create_time_; }
  const QDateTime GetDeadline() const { return deadline_; }
  const QTime GetTimeLimit() const { return time_; }
  const QString GetTaskName() const { return task_name_; }
};
} // end namespace db

#endif  // TASK_H
