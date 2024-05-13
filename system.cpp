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

  q.exec("SELECT LAST_INSERT_ID()");
  if (q.next()) {
    return q.value(0);
  } else {
    return QVariant();
  }
}

auto db::System::addWordBankRelation(QSqlQuery &q,
                    const qint64 &word_bank_id,
                    const qint64 &word_id) ->QVariant {
  q.addBindValue(word_bank_id);
  q.addBindValue(word_id);
  q.exec();

  q.exec("SELECT LAST_INSERT_ID()");
  if (q.next()) {
    return q.value(0);
  } else {
    return QVariant();
  }
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

auto db::System::returnUnlearnedWord(QSqlQuery &q, const qint64 &student_id, const qint64 &word_bank_id) -> QList<db::WordInfo>
{
  QList<WordInfo> wordList;

  q.addBindValue(student_id);
  q.addBindValue(word_bank_id);
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

QList<db::WordBankInfo> db::System::returnWordBank(QSqlQuery &q)
{
  QList<WordBankInfo> bankList;

  if (!q.exec()) {
    qDebug() << "Error executing returnWordBank:" << q.lastError().text();
    return bankList; // 返回空列表
  }
  while (q.next()) {
    WordBankInfo bankInfo;
    bankInfo.id = q.value(0).toLongLong();
    bankInfo.name = q.value(1).toString();
    bankInfo.picture_url = q.value(2).toString();
    bankList.append(bankInfo);
  }

  return bankList;
}

auto db::System::returnBankList(QSqlQuery &q, const qint64 &bank_id) -> QList<WordInfo>
{
  QList<WordInfo> wordList;
  q.addBindValue(bank_id);
  if (!q.exec()) {
    qDebug() << "Error executing returnBankList:" << q.lastError().text();
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

QVariant db::System::returnTeacherNameInfo(QSqlQuery &q, const qint64 &teacher_id)
{
  q.addBindValue(teacher_id);
  QVariant name;
  if (!q.exec()) {
    qDebug() << "Error executing returnTeacherNameInfo:" << q.lastError().text();
    return name; // 返回空列表
  }
  if (q.next()) {
    name = q.value("name");
    qDebug() << "Teacher name: " << name;
    return name;
  } else {
    qDebug() << "No teacher_name found in tasktable";
    return 0;
  }
}

QVariant db::System::returnStudentNameInfo(QSqlQuery &q, const qint64 &student_id)
{
  q.addBindValue(student_id);
  QVariant name;
  if (!q.exec()) {
    qDebug() << "Error executing returnStudentNameInfo:" << q.lastError().text();
    return name; // 返回空列表
  }
  if (q.next()) {
    name = q.value("name");
    qDebug() << "Student name: " << name;
    return name;
  } else {
    qDebug() << "No student_name found in tasktable";
    return 0;
  }
}

//=============== semantics movements =================//
auto db::System::returnTaskNumber() -> int
{
  QSqlQuery query(returnDatabase());
  if(!query.prepare(returnTaskN)) {
    throw std::runtime_error("Failed to returnTaskNumber sql");
  }

  if (!query.exec()) {
    qDebug() << "Error executing returnTaskNumber:" << query.lastError().text();
    return -1;
  }
  if (query.next()) {
    int totalCount = query.value("total_count").toInt();
    qDebug() << "Total task count:" << totalCount;
    return totalCount;
  } else {
    qDebug() << "No records found in tasktable";
    return 0;
  }
}

auto db::System::returnWordNumber() -> int
{
  QSqlQuery query(returnDatabase());
  if(!query.prepare(returnWordN)) {
    throw std::runtime_error("Failed to returnWordNumber sql");
  }

  if (!query.exec()) {
    qDebug() << "Error executing returnWordNumber:" << query.lastError().text();
    return -1;
  }
  if (query.next()) {
    int totalCount = query.value("total_count").toInt();
    qDebug() << "Total task count:" << totalCount;
    return totalCount;
  } else {
    qDebug() << "No records found in words";
    return 0;
  }
}

auto db::System::returnWordBankNumber() -> int
{
  QSqlQuery query(returnDatabase());
  if(!query.prepare(returnWordBankN)) {
    throw std::runtime_error("Failed to returnWordBankNumber sql");
  }

  if (!query.exec()) {
    qDebug() << "Error executing returnWordBankNumber:" << query.lastError().text();
    return -1;
  }
  if (query.next()) {
    int totalCount = query.value("total_count").toInt();
    qDebug() << "Total task count:" << totalCount;
    return totalCount;
  } else {
    qDebug() << "No records found in wordtable";
    return 0;
  }
}

auto db::System::returnStudentsLearnedNumber(const qint64 &student_id) -> int
{
  QSqlQuery query(returnDatabase());
  if(!query.prepare(returnStudentsLearnedN)) {
    throw std::runtime_error("Failed to returnStudentsLearnedNumber sql");
  }
  query.addBindValue(student_id);

  if (!query.exec()) {
    qDebug() << "Error executing returnStudentsLearnedNumber:" << query.lastError().text();
    return -1;
  }
  if (query.next()) {
    int totalCount = query.value("total_count").toInt();
    qDebug() << "Total task count:" << totalCount;
    return totalCount;
  } else {
    qDebug() << "No records found in wordtable";
    return 0;
  }
}

auto db::System::returnWordBankWordNumber(const qint64 &word_bank_id) -> int
{
  QSqlQuery query(returnDatabase());
  if(!query.prepare(returnWordBankLearned)) {
    throw std::runtime_error("Failed to returnWordBankWordNumber sql");
  }
  query.addBindValue(word_bank_id);

  if (!query.exec()) {
    qDebug() << "Error executing returnWordBankWordNumber:" << query.lastError().text();
    return -1;
  }
  if (query.next()) {
    int totalCount = query.value("word_count").toInt();
    qDebug() << "Total word count:" << totalCount;
    return totalCount;
  } else {
    qDebug() << "No records found in wordtable";
    return 0;
  }
}

auto db::System::returnStudentWordBankLearned(const qint64 &word_bank_id, const qint64 &student_id) -> int
{
  QSqlQuery query(returnDatabase());
  if(!query.prepare(studentLearnedWordBank)) {
    throw std::runtime_error("Failed to returnStudentWordBankLearned sql");
  }
  query.addBindValue(student_id);
  query.addBindValue(word_bank_id);

  if (!query.exec()) {
    qDebug() << "Error executing returnStudentWordBankLearned:" << query.lastError().text();
    return -1;
  }
  if (query.next()) {
    int totalCount = query.value("learned_word_count").toInt();
    qDebug() << "Total word count:" << totalCount;
    return totalCount;
  } else {
    qDebug() << "No records found in wordtable";
    return 0;
  }
}

double db::System::returnLearnedRate(const qint64 &student_id)
{
  return (double)(returnStudentsLearnedNumber(student_id))/(returnWordNumber());
}

double db::System::returnLearnedRateForWordBank(const qint64 &student_id, const qint64 &word_bank_id)
{
  return (double)(returnStudentWordBankLearned(word_bank_id, student_id)) / (returnWordBankWordNumber(word_bank_id));
}

auto db::System::returnWordBankInfo() -> QList<db::WordBankInfo>
{
  QSqlQuery query(returnDatabase());
  if(!query.prepare(returnTotalWordBank)) {
    throw std::runtime_error("Failed to prepare returnWordBankInfo insert sql");
  }

  return returnWordBank(query);
}

auto db::System::returnWordBankList(const qint64 &word_bank_id) -> QList<db::WordInfo>
{
  QSqlQuery query(returnDatabase());
  if(!query.prepare(returnWordBankListInfo)) {
    throw std::runtime_error("Failed to prepare returnWordBankList insert sql");
  }

  return returnBankList(query, word_bank_id);
}

void db::System::importAudioDB() {
  QDir directory("../word-memorize-software/audios");
  QFileInfoList files = directory.entryInfoList(QStringList() << "*.mp3", QDir::Files);

  for (const QFileInfo &fileInfo : files) {
    QString filePath = fileInfo.filePath();
    QString fileName = fileInfo.fileName();
    QString name = fileName.split(".mp3").first();

    storeMp3File(filePath, name);
  }
}

QVariant db::System::returnTeacherInfo(const qint64 &teacher_id)
{
  QSqlQuery query(returnDatabase());
  if(!query.prepare(retrunTeacherName)) {
    throw std::runtime_error("Failed to prepare returnTeacherInfo insert sql");
  }

  return returnTeacherNameInfo(query, teacher_id);
}

QVariant db::System::returnStudentInfo(const qint64 &student_id)
{
  QSqlQuery query(returnDatabase());
  if(!query.prepare(returnStudentName)) {
    throw std::runtime_error("Failed to prepare returnStudentInfo insert sql");
  }

  return returnStudentNameInfo(query, student_id);
}

auto db::System::storeMp3File(const QString &filePath, const QString &name) -> void
{
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly)) {
    qDebug() << "Failed to open file" << filePath;
    return;
  }
  //qDebug() << filePath;

  QByteArray fileData = file.readAll();
  file.close();
  //qDebug() <<fileData;

  QSqlQuery query(returnDatabase());
  query.prepare("INSERT INTO radio (name, audio) VALUES (:name, :audio)");
  query.bindValue(":name", name);
  query.bindValue(":audio", fileData);
  if (!query.exec()) {
    qDebug() << "Failed to insert file" << filePath << "into database:" << query.lastError().text();
  }
}

auto db::System::runMp3(const QString &name) -> QString
{
  QSqlQuery query(returnDatabase());

  query.prepare("SELECT audio FROM radio WHERE name = :name");
  query.bindValue(":name", name);
  if (!query.exec() || !query.first()) {
    qDebug() << "Failed to retrieve name" << name << "from database:" << query.lastError().text();
    return nullptr;
  }

  QByteArray fileData = query.value(0).toByteArray();

  QString filename = name + ".mp3";
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly)) {
    qDebug() << "Failed to open file for writing";
    return filename;
  }

  file.write(fileData); // 将数据写入文件
  file.close(); // 关闭文件

  return filename;
}


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

auto db::System::generateWords(const qint64 &student_id, const qint64 &word_bank_id, const qint64 plan) -> QList<WordInfo> {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(returnUnLearnedWords)) {
    throw std::runtime_error("Failed to prepare returnUnLearnedWords sql");
  }

  QList<WordInfo> unlearned_words = returnUnlearnedWord(query, student_id, word_bank_id);
  int count = 0;
  QList<WordInfo> generate_words;
  for(const auto &word : unlearned_words) {
    if(count == plan) {
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
  qint64 word_bank_number_ = returnWordBankNumber();
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
        qint64 word_number = returnWordNumber();
        importWordBank(word_bank_number_, word_number, word, mean, phonetic_symbol, "bbbbb");
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

