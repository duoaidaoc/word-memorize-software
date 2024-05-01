#ifndef CLASS_H
#define CLASS_H

#include "table.h"
#include <QString>
#include <QVariant>

namespace db {
class Class : Table{
private:
  qint64 class_id_;
  QString class_name_;
  QString class_cue_;

  const QLatin1String insertClassTable = QLatin1String(R"(
        insert into class(id, name, cue) values(?, ?, ?)
    )");

  static QVariant addClass(QSqlQuery &q, const qint64 &id, const QString &name, const QString &cue);

public:
  explicit Class(Database& db) : Table(db) {}
  QVariant registerClass();

  void SetId(const qint64& id) { class_id_ = id; }
  void SetName(const QString& name) { class_name_ = name; }
  void SetCue(const QString& cue) { class_cue_ = cue; }

  const qint64 GetId() const { return class_id_; }
  const QString GetName() const { return class_name_; }
  const QString GetCue() const { return class_cue_; }
};
} // end namespace db

#endif  // CLASS_H
