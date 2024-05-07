#ifndef SYSTEM_H
#define SYSTEM_H

#include "table.h"
#include "role.h"
#include <QString>
#include <QVariant>

namespace db {
class System {
private:
  Database &db_;

  const QLatin1String insertWordBook = QLatin1String(R"(
        insert into wordbank(id, name, picture_url) values(?, ?, ?)
    )");
  const QLatin1String insertWordBankRelation = QLatin1String(R"(
        insert into WordBankRelationTable(word_bank_id, word_id) values(?, ?)
    )");
  const QLatin1String studentsLoginPassword = QLatin1String(R"(
        SELECT password from students where id = ?
    )");
  const QLatin1String teachersLoginPassword = QLatin1String(R"(
        SELECT password from teachers where id = ?
    )");

  static QVariant addWordBook(QSqlQuery &q, const qint64 &word_bank_id, const QString &name, const QString &picture_url);
  static QVariant addWordBankRelation(QSqlQuery &q, const qint64 &word_bank_id, const qint64 &word_id);
  static QVariant returnPassword(QSqlQuery &q, const qint64 &id);

public:
  explicit System(Database &db) : db_(db) {}
  QVariant createWordBank(const qint64 &id, const QString &name, const QString &pircture_url);
  QVariant importWordBank(const qint64 &word_bank_id,
                          const qint64 &word_id,
                          const QString &english,
                          const QString &chinese,
                          const QString &phonetic,
                          const QString &audio_url);

  auto returnDatabase() -> QSqlDatabase& {
    return db_.returnDatabase();
  }

  auto returnDB() -> Database& {
    return db_;
  }

  auto returnTeacherPassword(const qint64 &teacher_id) -> QVariant;
  auto returnStudentPassword(const qint64 &student_id) -> QVariant;
};
} // end namespace db

#endif  // SYSTEM_H
