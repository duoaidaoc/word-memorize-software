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
  //int word_number_;              // 给每个单词赋予独立的id。
  //int word_bank_number_;
  //int task_number_;

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
        ) AND id IN (
          SELECT word_id
          FROM WordBankRelationTable
          WHERE word_bank_id = ?
        )
        ORDER BY english;
    )");
  const QLatin1String returnTaskN = QLatin1String(R"(
        SELECT COUNT(*) AS total_count FROM tasktable
    )");
  const QLatin1String returnWordN = QLatin1String(R"(
        SELECT COUNT(*) AS total_count FROM words
    )");
  const QLatin1String returnWordBankN = QLatin1String(R"(
        SELECT COUNT(*) AS total_count FROM wordbank
    )");
  const QLatin1String returnStudentsLearnedN = QLatin1String(R"(
        SELECT COUNT(*) AS total_count FROM StudentSysLearn WHERE student_id = ?
    )");
  const QLatin1String returnWordBankLearned = QLatin1String(R"(
        SELECT COUNT(*) AS word_count
        FROM WordBankRelationTable
        WHERE word_bank_id = ?
    )");
  const QLatin1String studentLearnedWordBank = QLatin1String(R"(
        SELECT COUNT(*) AS learned_word_count
        FROM StudentSysLearn
        WHERE student_id = ? AND word_id IN (
            SELECT word_id FROM WordBankRelationTable WHERE word_bank_id = ?
        )
    )");
  const QLatin1String returnTotalWordBank = QLatin1String(R"(
        SELECT * FROM wordbank
    )");
  const QLatin1String returnWordBankListInfo = QLatin1String(R"(
        SELECT *
        FROM words JOIN WordBankRelationTable ON words.id = WordBankRelationTable.word_id
        WHERE WordBankRelationTable.word_bank_id = ?
  )");
  //const QLatin1String return = QLatin1String(R"(
  //      SELECT * FROM wordbank
  //  )");
  //const QLatin1String returnTotalWordBank = QLatin1String(R"(
  //      SELECT * FROM wordbank
  //  )");

  static QVariant addWordBook(QSqlQuery &q, const qint64 &word_bank_id, const QString &name, const QString &picture_url);
  static QVariant addWordBankRelation(QSqlQuery &q, const qint64 &word_bank_id, const qint64 &word_id);
  static QVariant returnPassword(QSqlQuery &q, const qint64 &id);
  static QList<WordInfo> returnUnlearnedWord(QSqlQuery &q, const qint64 &student_id, const qint64 &word_bank_id);
  static QList<WordBankInfo> returnWordBank(QSqlQuery &q);
  static QList<WordInfo> returnBankList(QSqlQuery &q, const qint64 &bank_id);

public:
  explicit System(Database &db) : db_(db) {  }
  bool importLocalWords(const QString &filename);
  int checkAlreadyInWords(const QString &word);
  QVariant createWordBank(const qint64 &id, const QString &name, const QString &pircture_url);
  QVariant importWordBank(const qint64 &word_bank_id,
                          const qint64 &word_id,
                          const QString &english,
                          const QString &chinese,
                          const QString &phonetic,
                          const QString &audio_url);
  QList<WordInfo> generateWords(const qint64 &stduent_id, const qint64 &word_bank_id, const qint64 plan);

  auto returnDatabase() -> QSqlDatabase& {
    return db_.returnDatabase();
  }
  auto returnDB() -> Database& {
    return db_;
  }

  auto returnTeacherPassword(const qint64 &teacher_id) -> QVariant;
  auto returnStudentPassword(const qint64 &student_id) -> QVariant;
  auto returnTaskNumber() -> int;
  auto returnWordNumber() -> int;
  auto returnWordBankNumber() -> int;
  auto returnStudentsLearnedNumber(const qint64 &student_id) -> int;
  auto returnWordBankWordNumber(const qint64 &word_bank_id) -> int;
  auto returnStudentWordBankLearned(const qint64 &word_bank_id, const qint64 &student_id) -> int;

  auto returnLearnedRate(const qint64 &student_id) -> double;
  auto returnLearnedRateForWordBank(const qint64 &student_id, const qint64 &word_bank_id) -> double;

  auto returnWordBankInfo() -> QList<WordBankInfo>;
  auto returnWordBankList(const qint64 &word_bank_id) -> QList<WordInfo>;

  //auto returnStudentName(const qint64 &student_id) -> QVariant();
  //auto returnTeacherName(const qint64 &teacher_id) -> QVariant();
};
} // end namespace db

#endif  // SYSTEM_H
