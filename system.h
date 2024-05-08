#ifndef SYSTEM_H
#define SYSTEM_H

#include "table.h"
#include "words.h"
#include "util.h"
#include <QString>
#include <QVariant>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace db {
class System {
private:
  Database &db_;
  int word_number_;              // 给每个单词赋予独立的id。
  int word_bank_number_;
  int task_number_;

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
  const QLatin1String wordIdInWords = QLatin1String(R"(
        SELECT id from words where english = ?
    )");
  const QLatin1String returnUnLearnedWords = QLatin1String(R"(
        SELECT id, english, chinese, phonetic, audio_url
        FROM words
        WHERE id NOT IN (
          SELECT word_id
          FROM StudentSysLearn
          WHERE student_id = ?
        )
        ORDER BY english;
    )");

  static QVariant addWordBook(QSqlQuery &q, const qint64 &word_bank_id, const QString &name, const QString &picture_url);
  static QVariant addWordBankRelation(QSqlQuery &q, const qint64 &word_bank_id, const qint64 &word_id);
  static QVariant returnPassword(QSqlQuery &q, const qint64 &id);
  static QList<WordInfo> returnUnlearnedWord(QSqlQuery &q, const qint64 &student_id);
public:
  explicit System(Database &db) : db_(db) { word_number_ = 0; word_bank_number_ = 0; task_number_ = 0; }
  bool importLocalWords(const QString &filename);
  int checkAlreadyInWords(const QString &word);
  QVariant createWordBank(const qint64 &id, const QString &name, const QString &pircture_url);
  QVariant importWordBank(const qint64 &word_bank_id,
                          const qint64 &word_id,
                          const QString &english,
                          const QString &chinese,
                          const QString &phonetic,
                          const QString &audio_url);
  QList<WordInfo> generateWords(const qint64 &stduent_id);

  auto returnDatabase() -> QSqlDatabase& {
    return db_.returnDatabase();
  }
  auto returnDB() -> Database& {
    return db_;
  }
  auto returnWordNumber() -> int {
    return word_number_;
  }
  auto returnWordBankNumber() -> int {
    return word_bank_number_;
  }
  auto returnTaskNumber() -> int {
    return task_number_;
  }

  void incTaskNumber() {
    task_number_ ++;
  }

  auto returnTeacherPassword(const qint64 &teacher_id) -> QVariant;
  auto returnStudentPassword(const qint64 &student_id) -> QVariant;
};
} // end namespace db

#endif  // SYSTEM_H
