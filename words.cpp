#include "words.h"

//--------------------------- bind parameters --------------------------//
auto db::Word::addWord(QSqlQuery& q,
                        const qint64& word_id,
                        const QString& english,
                        const QString& chinese,
                        const QString& phonetic,
                      const QString& audio_url) -> QVariant {
  q.addBindValue(word_id);
  q.addBindValue(english);
  q.addBindValue(chinese);
  q.addBindValue(phonetic);
  q.addBindValue(audio_url);
  q.exec();

  return q.lastInsertId();
}

//--------------------------- semantic functions --------------------------//
// 增删改查
auto db::Word::registerWord() -> QVariant {
  // 初始化query。
  QSqlQuery query(returnDatabase());
  if(!query.prepare(insertWordTable)) {
    throw std::runtime_error("Failed to prepare WordTable insert sql");
  }

  return addWord(query, GetId(), GetEnglish(), GetChinese(), GetPhonetic(), GetAudioUrl());
}
