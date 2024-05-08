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

auto db::System::returnUnlearnedWord(QSqlQuery &q, const qint64 &student_id) -> QList<db::WordInfo>
{
  QList<WordInfo> wordList;

  q.addBindValue(student_id);
  if (!q.exec()) {
    qDebug() << "Error executing returnUnlearnedWord:" << q.lastError().text();
    return wordList; // 返回空列表
  }
  while (q.next()) {
    WordInfo wordInfo;
    wordInfo.word_id = q.value(0).toLongLong();
    wordInfo.english = q.value(1).toString();
    wordInfo.chinese = q.value(2).toString();
    wordInfo.phonetic = q.value(3).toString();
    wordInfo.audio_url = q.value(4).toString();
    wordList.append(wordInfo);
  }

  return wordList;
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

auto db::System::generateWords(const qint64 &student_id) -> QList<WordInfo> {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(returnUnLearnedWords)) {
    throw std::runtime_error("Failed to prepare returnUnLearnedWords sql");
  }

  QList<WordInfo> unlearned_words = returnUnlearnedWord(query, student_id);
  int count = 0;
  QList<WordInfo> generate_words;
  for(const auto &word : unlearned_words) {
    if(count == 20) {
      break;
    }

    generate_words.append(word);
    count++;
  }
  return generate_words;
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

bool db::System::importLocalWords(const QString &filename) {
  QString jsonFileName = QCoreApplication::applicationDirPath() + "/../word-memorize-software/" + filename + ".json";
  QFile file(jsonFileName);
  qDebug() << "====================" << jsonFileName;
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Could not open file for reading";
    return false;
  }

  QByteArray rawData = file.readAll();
  file.close();

  // 解析JSON数据
  QJsonParseError error;
  QJsonDocument doc = QJsonDocument::fromJson(rawData, &error);
  if (error.error != QJsonParseError::NoError) {
    qDebug() << "Error parsing JSON:" << error.errorString();
    return false;
  }

  if (!doc.isArray()) {
    qDebug() << "JSON document is not an array";
    return false;
  }

  // 获取JSON数组
  QJsonArray jsonArray = doc.array();
  word_bank_number_ ++;
  createWordBank(word_bank_number_, filename, "djdjdjjdjdjdjdj");

  // 循环处理每个单词对象
  foreach(const QJsonValue & value, jsonArray) {
    if (value.isObject()) {
      QJsonObject jsonObj = value.toObject();

      // 提取所需字段
      QString mean = jsonObj["mean"].toString();
      QString phonetic_symbol = jsonObj["phonetic_symbol"].toString();
      QString word = jsonObj["word"].toString();

      qDebug() << "-----------------------";
      qDebug() << "Word:" << word;
      qDebug() << "Meaning:" << mean;
      qDebug() << "Phonetic Symbol:" << phonetic_symbol;
      qDebug() << "-----------------------";
      if(checkAlreadyInWords(word) == -1) {
        importWordBank(word_bank_number_, word_number_++, word, mean, phonetic_symbol, "bbbbb");
      }
    }
  }

  return true;
}

auto db::System::checkAlreadyInWords(const QString &word) -> int {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(wordIdInWords)) {
    throw std::runtime_error("Failed to prepare wordInWords sql");
  }

  query.addBindValue(word);
  if(!query.exec()) {
    throw std::runtime_error("Failed to execute query");
  }

  if (query.next()) {
    return query.value(0).toInt();
  } else {
    return -1;
  }
}

