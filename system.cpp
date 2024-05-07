#include "system.h"

//=============== bind parameters =================//
auto db::System::addWordBook(QSqlQuery &q,
                 const qint64 &word_bank_id,
                 const QString &name,
                 const QString &picture_url) -> QVariant {
  q.addBindValue(word_bank_id);
  q.addBindValue(name);
  q.addBindValue(picture_url);
  q.exec();

  return q.lastInsertId();
}

auto db::System::addWordBankRelation(QSqlQuery &q,
                    const qint64 &word_bank_id,
                    const qint64 &word_id) ->QVariant {
  q.addBindValue(word_bank_id);
  q.addBindValue(word_id);
  q.exec();

  return q.lastInsertId();
}

auto db::System::returnPassword(QSqlQuery &q, const qint64 &id) -> QVariant {
  q.addBindValue(id);
  q.exec();
  QString password;
  if (q.next()) { // 如果有查询结果
    // 获取密码字段的值
    password = q.value("password").toString();
  }
  // 否则，password是空，判断空即可。

  return password;
}

//=============== semantics movements =================//
auto db::System::createWordBank(const qint64 &id, const QString &name, const QString &picture_url) -> QVariant {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(insertWordBook)) {
    throw std::runtime_error("Failed to prepare createWordBank insert sql");
  }

  return addWordBook(query, id, name, picture_url);
}

auto db::System::importWordBank(const qint64 &word_bank_id,
                        const qint64 &word_id,
                        const QString &english,
                        const QString &chinese,
                        const QString &phonetic,
                        const QString &audio_url) -> QVariant {
  Word word_(returnDB());
  word_.SetId(word_id);
  word_.SetEnglish(english);
  word_.SetChinese(chinese);
  word_.SetPhonetic(phonetic);
  word_.SetAudioUrl(audio_url);
  word_.registerWord();

  QSqlQuery query(returnDatabase());
  if(!query.prepare(insertWordBankRelation)) {
    throw std::runtime_error("Failed to prepare importWordBank insert sql");
  }

  return addWordBankRelation(query, word_bank_id, word_id);
}

auto db::System::returnTeacherPassword(const qint64 &teacher_id) -> QVariant {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(teachersLoginPassword)) {
    throw std::runtime_error("Failed to prepare returnTeacherPasswor sql");
  }

  return returnPassword(query, teacher_id);
}

auto db::System::returnStudentPassword(const qint64 &student_id) -> QVariant {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(studentsLoginPassword)) {
    throw std::runtime_error("Failed to prepare returnStudentPassword sql");
  }

  return returnPassword(query, student_id);
}
