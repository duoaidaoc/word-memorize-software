#ifndef WORDS_H
#define WORDS_H

#include "table.h"
#include <QString>
#include <QVariant>

/*
CREATE TABLE IF NOT EXISTS words (
    id BIGINT PRIMARY KEY,
    english   VARCHAR(255),
    chinese   VARCHAR(255),
    phonetic  VARCHAR(255),
    audio_url VARCHAR(255)
    )
*/

namespace db {
class Word : Table{
private:
  qint64 word_id_;
  QString english_;
  QString chinese_;
  QString phonetic_;
  QString audio_url_;

  const QLatin1String insertWordTable = QLatin1String(R"(
        insert into words(id, english, chinese, phonetic, audio_url) values(?, ?, ?, ?, ?)
    )");

  static QVariant addWord(QSqlQuery& q,
                          const qint64& word_id,
                          const QString& english,
                          const QString& chinese,
                          const QString& phonetic,
                          const QString& audio_url);

public:
  explicit Word(Database& db) : Table(db) {}
  QVariant registerWord();

  void SetId(const qint64& id) { word_id_ = id; }
  void SetEnglish(const QString& english) { english_ = english; }
  void SetChinese(const QString& chinese) { chinese_ = chinese; }
  void SetPhonetic(const QString& phonetic) { phonetic_ = phonetic; }
  void SetAudioUrl(const QString& audiourl) { audio_url_ = audiourl; }

  const qint64 GetId() const { return word_id_; }
  const QString GetEnglish() const { return english_; }
  const QString GetChinese() const { return chinese_; }
  const QString GetPhonetic() const { return phonetic_; }
  const QString GetAudioUrl() const { return audio_url_; }
};
} // end namespace db

#endif  // WORDS_H
