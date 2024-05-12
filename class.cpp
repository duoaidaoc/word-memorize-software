#include "class.h"

//--------------------------- bind parameters --------------------------//
auto db::Class::addClass(QSqlQuery &q,
                         const qint64 &id,
                         const QString &name,
                         const QString &cue) -> QVariant {
  q.addBindValue(id);
  q.addBindValue(name);
  q.addBindValue(cue);
  q.exec();

  q.exec("SELECT LAST_INSERT_ID()");
  if (q.next()) {
    return q.value(0);
  } else {
    return QVariant();
  }
}

//--------------------------- semantic functions --------------------------//
// 增删改查
auto db::Class::registerClass() -> QVariant {
  // 初始化query。
  QSqlQuery query(returnDatabase());
  if(!query.prepare(insertClassTable)) {
    throw std::runtime_error("Failed to prepare TearcherTable insert sql");
  }

  return addClass(query, GetId(), GetName(), GetCue());
}
