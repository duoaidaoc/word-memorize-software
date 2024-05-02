#include "task.h"

//--------------------------- bind parameters --------------------------//
auto db::Task::addTask(QSqlQuery &q,
                       const qint64 &task_id,
                       const QDateTime& create_time,
                       const QDateTime& deadline,
                       const QTime& time) -> QVariant {
  q.addBindValue(task_id);
  q.addBindValue(create_time);
  q.addBindValue(deadline);
  q.addBindValue(time);
  q.exec();

  return q.lastInsertId();
}

//--------------------------- semantic functions --------------------------//
// 增删改查
auto db::Task::registerTask() -> QVariant {
  // 初始化query。
  QSqlQuery query(returnDatabase());
  if(!query.prepare(insertTaskTable)) {
    throw std::runtime_error("Failed to prepare TearcherTable insert sql");
  }

  return addTask(query, GetId(), GetCreateTime(), GetDeadline(), GetTimeLimit());
}
