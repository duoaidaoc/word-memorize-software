#include "role.h"
#include "resource_manager.h"

/*
    qint64 id_;
    QString name_;
    QString password_;
    QString profile_photo_url_;
 */
//====================================== Role part =====================================//
auto db::Role::displayTaskInClass(QSqlQuery &q, const qint64 &class_id) -> QList<TaskInfo> {
  QList<TaskInfo> taskList;

  q.addBindValue(class_id);
  if (!q.exec()) {
    qDebug() << "Error executing displayTaskInClass query:" << q.lastError().text();
    return taskList; // 返回空列表
  }

  if (!q.next()) {
    qDebug() << "No task members for class with ID:" << class_id;
    return taskList; // 返回空列表
  }

  do {
    TaskInfo taskInfo;
    taskInfo.taskId = q.value("id").toLongLong();
    taskInfo.create_time = q.value("create_time").toDateTime();
    taskInfo.deadline = q.value("deadline").toDateTime();
    taskInfo.time = q.value("time_limit").toTime();
    taskList.append(taskInfo);
  } while (q.next());

  return taskList;
}

auto db::Role::displayWordFromTask(QSqlQuery &q, const qint64 &task_id) -> QList<db::WordInfo>
{
  QList<WordInfo> wordList;

  q.addBindValue(task_id);
  if (!q.exec()) {
    qDebug() << "Error executing displayWordFromTask query:" << q.lastError().text();
    return wordList; // 返回空列表
  }

  if (!q.next()) {
    qDebug() << "No words for task with ID:" << task_id;
    return wordList; // 返回空列表
  }

  do {
    WordInfo wordInfo;
    wordInfo.word_id = q.value("id").toLongLong();
    wordInfo.english = q.value("english").toString();
    wordInfo.chinese = q.value("chinese").toString();
    wordInfo.phonetic = q.value("phonetic").toString();
    wordInfo.audio_url = q.value("audio_url").toString();
    wordList.append(wordInfo);
  } while (q.next());


  return wordList;
}

void db::Teacher::displayTeacher(QSqlQuery &q, const qint64 &id) {
  q.addBindValue(id);
  if (!q.exec()) {
    qWarning() << "Query failed:" << q.lastError().text();
  } else {
    qDebug() <<"------ 打印老师信息 -----\n";
    while (q.next()) {
      // 获取每一列的值
      qint64 id = q.value("id").toLongLong();
      QString name = q.value("name").toString();
      QString profilePhotoUrl = q.value("profile_photo_url").toString();

      qDebug() << "id: " << id << "name: " << name << "profilePhotoUrl: " << profilePhotoUrl << "\n";
    }
    qDebug() <<"------ 结束打印老师信息 -----\n";
  }
}

auto db::Teacher::displayTeacherClass(QSqlQuery &q, const qint64 &teacher_id) ->QList<QPair<qint64, QString>> {
  QList<QPair<qint64, QString>> classesList;

  q.addBindValue(teacher_id);
  if (!q.exec()) {
    qDebug() << "Error executing displayTeacherClass:" << q.lastError().text();
    return classesList; // 返回空列表
  }
  while (q.next()) {
    qint64 classId = q.value("id").toLongLong();
    QString className = q.value("name").toString();
    classesList.append(qMakePair(classId, className));
  }

  return classesList;
}

auto db::Role::displayClassTeacher(QSqlQuery &q, const qint64 &class_id) -> QList<TeacherInfo> {
  QList<TeacherInfo> teachersList;

  q.addBindValue(class_id);
  if (!q.exec()) {
    qDebug() << "Error executing displayClassTeacher query:" << q.lastError().text();
    return teachersList; // 返回空列表
  }

  if (!q.next()) {
    qDebug() << "No teachers found for class with ID:" << class_id;
    return teachersList; // 返回空列表
  }

  do {
    TeacherInfo teacherInfo;
    teacherInfo.teacherId = q.value("id").toLongLong();
    teacherInfo.teacherName = q.value("name").toString();
    teacherInfo.teacherUrl = q.value("profile_photo_url").toString();
    teachersList.append(teacherInfo);
  } while (q.next());

  return teachersList;
}

auto db::Role::displayClassMember(QSqlQuery &q, const qint64 &class_id) -> QList<StudentInfo> {
  QList<StudentInfo> studentList;

  q.addBindValue(class_id);
  if (!q.exec()) {
    qDebug() << "Error executing displayClassMember query:" << q.lastError().text();
    return studentList; // 返回空列表
  }

  if (!q.next()) {
    qDebug() << "No student members for class with ID:" << class_id;
    return studentList; // 返回空列表
  }

  do {
    StudentInfo memberInfo;
    memberInfo.studentId = q.value("id").toLongLong();
    memberInfo.studentName = q.value("name").toString();
    memberInfo.studentUrl = q.value("profile_photo_url").toString();
    studentList.append(memberInfo);
  } while (q.next());

  return studentList;
}

auto db::Role::calculateTotal(QSqlQuery &q, const qint64 &task_id) -> QList<WordInfo>
{
  QList<WordInfo> wordList;

  q.addBindValue(task_id);
  if (!q.exec()) {
    qDebug() << "Error executing calculateTotal query:" << q.lastError().text();
    return wordList; // 返回空列表
  }

  if (!q.next()) {
    qDebug() << "No word for taso with " << task_id;
    return wordList; // 返回空列表
  }

  do {
    WordInfo wordInfo;
    wordInfo.word_id = q.value("id").toLongLong();
    wordInfo.english = q.value("english").toString();
    wordInfo.chinese = q.value("chinese").toString();
    wordInfo.phonetic = q.value("phonetic").toString();
    wordInfo.audio_url = q.value("audio_url").toString();
    wordList.append(wordInfo);
  } while (q.next());

  return wordList;
}

auto db::Role::isWordLearned(QSqlQuery &q, const WordInfo &wordInfo, const qint64 &student_id) -> bool
{
  q.addBindValue(wordInfo.word_id);
  q.addBindValue(student_id);
  if (!q.exec()) {
    qDebug() << "Error executing isWordLearned query:" << q.lastError().text();
    return false;
  }

  if (!q.next()) {
    return false;
  }
  else {
    return true;
  }
}

//========================== semantic
auto db::Role::infoTaskInClass(const qint64 &class_id) -> QList<TaskInfo> {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(retrieveTaskInfo)) {
    throw std::runtime_error("Failed to infoTaskInClass sql");
  }

  QList<TaskInfo> taskInfo = displayTaskInClass(query, class_id);

  qDebug() <<"**************\n";
  for (const auto &task : taskInfo) {
    qDebug() << "Task ID:" << task.taskId << ", task start time:" << task.create_time  << ", task end_time:" << task.deadline << ", lasting time:" << task.time;
  }
  qDebug() <<"**************\n";

  return taskInfo;
}

auto db::Role::infoWordsInTask(const qint64 &task_id) -> QList<WordInfo> {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(retrieveWordFromTask)) {
    throw std::runtime_error("Failed to prepare retrieveWordFromTask sql");
  }

  QList<WordInfo> WordList = displayWordFromTask(query, task_id);

  qDebug() <<"**************\n";
  for (const auto &wordInfo : WordList) {
    qDebug() << "Wordid: " << wordInfo.word_id << "English:" << wordInfo.english << ", Chinese:" << wordInfo.chinese << ", Phonetic:" << wordInfo.phonetic << ", Audio URL:" << wordInfo.audio_url;
  }
  qDebug() <<"**************\n";

  return WordList;
}

auto db::Role::infoClassDetails(const qint64 &class_id) -> QList<TeacherInfo> {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(retrieveTeachersInClass)) {
    throw std::runtime_error("Failed to prepare student info class sql");
  }
  QList<TeacherInfo> classDetails = displayClassTeacher(query, class_id);

  qDebug() <<"**************\n";
  for (const auto &teacher : classDetails) {
    qDebug() << "Teacher ID:" << teacher.teacherId << ", Teacher Name:" << teacher.teacherName << ", Teacher url:" << teacher.teacherUrl;
  }
  qDebug() <<"**************\n";

  return classDetails;
}

auto db::Role::infoClassMembers(const qint64 &class_id) -> QList<StudentInfo> {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(retrieveClassMember)) {
    throw std::runtime_error("Failed to prepare student member class sql");
  }
  QList<StudentInfo> classMembers = displayClassMember(query, class_id);

  qDebug() <<"**************\n";
  for (const auto &student : classMembers) {
    qDebug() << "Student ID:" << student.studentId << ", Student Name:" << student.studentName << ", student url:" << student.studentUrl;
  }
  qDebug() <<"**************\n";

  return classMembers;
}

auto db::Role::infoTaskCondition(const qint64 &student_id, const qint64 &task_id) -> double
{
  QSqlQuery query(returnDatabase());
  if(!query.prepare(getTaskWords)) {
    throw std::runtime_error("Failed to prepare getTaskWords sql");
  }
  QList<WordInfo> totalWordList = calculateTotal(query, task_id);
  if(totalWordList.size() == 0) {
    qDebug() << "<<<<<<<<<<<<<<<<<< No words in task: " << task_id;
    return 0;
  }

  int wordLearned = 0;
  for(auto &word : totalWordList) {
    if(!query.prepare(isWordLearnedYet)) {
      throw std::runtime_error("Failed to prepare isWordLearnedYet sql");
    }
    if(isWordLearned(query, word, student_id)) {
      wordLearned ++;
    }
  }

  return ((double)wordLearned) / totalWordList.size();
}

//====================================== Teacher part =====================================//
//--------------------------- bind parameters --------------------------//
auto db::Teacher::addTeacher(QSqlQuery &q,
                             const qint64 &id,
                             const QString &name,
                             const QString &password,
                             const QString &profile_photo_url) -> QVariant {
  q.addBindValue(id);
  q.addBindValue(name);
  q.addBindValue(password);
  q.addBindValue(profile_photo_url);
  q.exec();

  return q.lastInsertId();
}

auto db::Teacher::deleteTeacher(QSqlQuery &q, const qint64 &id) -> bool {
  q.addBindValue(id);
  return q.exec();
}

auto db::Teacher::addTeacherClass(QSqlQuery &q,
                             const qint64 &teacher_id,
                             const qint64 &class_id) -> QVariant {
  q.addBindValue(teacher_id);
  q.addBindValue(class_id);
  q.exec();

  return q.lastInsertId();
}

auto db::Teacher::teacherDeleteClassTable(QSqlQuery &q, const qint64 &class_id) -> bool {
  q.addBindValue(class_id);
  if (!q.exec()) {
    qDebug() << "Delete operation failed:" << q.lastError().text();
    return false;  // 删除失败返回false
  }
  else {
    return true;
  }
}

auto db::Teacher::teacherDeleteTeacherClassTable(QSqlQuery &q, const qint64 &teacher_id, const qint64 &class_id) -> int {
  q.addBindValue(teacher_id);
  q.addBindValue(class_id);
  if (!q.exec()) {
    qDebug() << "Delete operation failed:" << q.lastError().text();
    return 0;
  }
  else {
    return q.numRowsAffected();
  }
}

auto db::Teacher::teacherDeleteStudentClassTable(QSqlQuery &q, const qint64 &class_id) -> bool {
  q.addBindValue(class_id);
  if (!q.exec()) {
    qDebug() << "Delete operation failed:" << q.lastError().text();
    return false;  // 删除失败返回false
  }
  else {
    return true;
  }
}

auto db::Teacher::addTeacherTaskClass(QSqlQuery &q,
                                  const qint64 &teacher_id,
                                  const qint64 &task_id,
                                  const qint64 &class_id) -> QVariant {
  q.addBindValue(teacher_id);
  q.addBindValue(task_id);
  q.addBindValue(class_id);
  q.exec();

  return q.lastInsertId();
}

auto db::Teacher::teacherDeleteAssignmentDistributionTable(QSqlQuery &q, const qint64 &teacher_id, const qint64 &task_id, const qint64 &class_id) -> int {
  q.addBindValue(teacher_id);
  q.addBindValue(task_id);
  q.addBindValue(class_id);
  if (!q.exec()) {
    qDebug() << "Delete operation failed:" << q.lastError().text();
    return 0;
  }
  else {
    return q.numRowsAffected();
  }
}

auto db::Teacher::teacherDeleteTaskTable(QSqlQuery &q, const qint64 &task_id) -> bool {
  q.addBindValue(task_id);
  if (!q.exec()) {
    qDebug() << "Delete operation failed:" << q.lastError().text();
    return false;  // 删除失败返回false
  }
  else {
    return true;
  }
}

auto db::Teacher::addTaskWord(QSqlQuery &q,
                              const qint64 &task_id,
                              const qint64 &word_id) -> QVariant {
  q.addBindValue(task_id);
  q.addBindValue(word_id);
  q.exec();

  return q.lastInsertId();
}

//--------------------------- semantic functions --------------------------//
// 增删改查
auto db::Teacher::registerRole() -> QVariant {
  // 初始化query。
  QSqlQuery query(returnDatabase());
  if(!query.prepare(insertTeacherTable)) {
    throw std::runtime_error("Failed to prepare TearcherTable insert sql");
  }

  return addTeacher(query, GetId(), GetName(), GetPassword(), GetProfilePhotoUrl());
}

auto db::Teacher::cancelRole() ->void {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(deleteTeacherRole)) {
    throw std::runtime_error("Failed to prepare TearcherTable cancel role sql");
  }

  if(!deleteTeacher(query, GetId())) {
    qWarning() << "Failed to canel Teacher: " << GetId() << "\n";
  }
}

auto db::Teacher::displayInfo() ->void {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(dispalyTeacherRole)) {
    throw std::runtime_error("Failed to prepare teacher display sql");
  }

  displayTeacher(query, GetId());
}

// 扩展操作
// @todo
auto db::Teacher::createClass(const qint64 &class_id, const QString &class_name, const QString &class_cue) -> QVariant {
  Class class_(returnDB());
  class_.SetId(class_id);
  class_.SetName(class_name);
  class_.SetCue(class_cue);

  // @todo 在老师class表格中插入条目。
  QSqlQuery query(returnDatabase());
  if(!query.prepare(insertTeacherClassTable)) {
    throw std::runtime_error("Failed to prepare TearcherClassTable insert sql");
  }
  addTeacherClass(query, GetId(), class_id);

  // 将创建的班级在班级表中插入。
  return class_.registerClass();
}

// @todo
// 需要删除三张表格：
// （1）class表格。
// （2）teacherclass表格里面所有带有class_id的。
// （3）studentclass表格里面所有带有class_id的。
auto db::Teacher::deleteClass(const qint64 &class_id) -> bool {
  QSqlQuery query(returnDatabase());

  if(!query.prepare(teacherDeleteTeacherClass)) {
    throw std::runtime_error("Failed to prepare teacherclass delete sql");
  }
  int rowsDeleted = teacherDeleteTeacherClassTable(query, GetId(), class_id);

  // 考虑到安全性，老师先删除老师班级表格，如果可以删除，说明班级存在，且老师有权限删除该表格。
  // 删除行数大于零，则老师有权限删除班级表格和学生班级表格。
  if(rowsDeleted > 0) {
    if(!query.prepare(teacherDeleteClass)) {
      throw std::runtime_error("Failed to prepare class delete sql");
    }
    if(!teacherDeleteClassTable(query, class_id)) {
      qWarning() << "Student failed to leave class: " << GetId() << "\n";
    }
    if(!query.prepare(teacherDeleteStudentClass)) {
      throw std::runtime_error("Failed to prepare studentclass delete sql");
    }
    return teacherDeleteStudentClassTable(query, class_id);
  }

  return false;
}

// 老师创建任务
auto db::Teacher::createTask(const qint64 &task_id,
                             const qint64 &class_id,
                             const QDateTime &create_time,
                             const QDateTime &deadline,
                             const QTime &time_limit) -> QVariant {
  Task task_(returnDB());
  task_.SetId(task_id);
  task_.SetCreateTime(create_time);
  task_.SetDeadline(deadline);
  task_.SetTime(time_limit);

  // @todo 在班级task中插入。
  QSqlQuery query(returnDatabase());
  if(!query.prepare(insertAssignmentDistribution)) {
    throw std::runtime_error("Failed to prepare TeacherClassTask insert sql");
  }
  addTeacherTaskClass(query, GetId(), task_id, class_id);

  // 将创建的班级在班级表中插入。
  return task_.registerTask();
}

// @todo
// 需要删除三张表格：
// （1）class表格。
// （2）teacherclass表格里面所有带有class_id的。
// （3）studentclass表格里面所有带有class_id的。
auto db::Teacher::deleteTask(const qint64 &task_id, const qint64 &class_id) -> bool {
  QSqlQuery query(returnDatabase());

  if(!query.prepare(teacherDeleteAssignmentDistribution)) {
    throw std::runtime_error("Failed to prepare assignmentDistribution delete sql");
  }
  int rowsDeleted = teacherDeleteAssignmentDistributionTable(query, GetId(), task_id, class_id);

  // 考虑到安全性，老师先删除老师班级表格，如果可以删除，说明班级存在，且老师有权限删除该表格。
  // 删除行数大于零，则老师有权限删除班级表格和学生班级表格。
  if(rowsDeleted > 0) {
    if(!query.prepare(teacherDeleteTask)) {
      throw std::runtime_error("Failed to prepare task delete sql");
    }
    return teacherDeleteTaskTable(query, task_id);
  }

  return false;
}

auto db::Teacher::createTaskWord(const qint64 &task_id, const qint64 &word_id) -> QVariant {
  // @todo 在班级task中插入。
  QSqlQuery query(returnDatabase());
  if(!query.prepare(insertTaskWordTable)) {
    throw std::runtime_error("Failed to prepare TaskWord insert sql");
  }

  return addTaskWord(query, task_id, word_id);
}

auto db::Teacher::infoTeacherClass() -> QList<QPair<qint64, QString>> {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(retrieveTeacherClasses)) {
    throw std::runtime_error("Failed to prepare student info class sql");
  }
  QList<QPair<qint64, QString>> classList = displayTeacherClass(query, GetId());

  qDebug() <<"**************\n";
  for (const auto &pair : classList) {
    qDebug() << "Class ID:" << pair.first << ", Class Name:" << pair.second;
  }
  qDebug() <<"**************\n";

  return classList;
}

auto db::Teacher::importTaskWordBank(const QList<QString> &englishList) -> int {
  // 向系统索要独一的task_id。
  auto man = resource_manager::getInstance();
  auto system = man->get_system();
  qint64 task_id = system.returnTaskNumber();
  QList<qint64> legalWords;
  for(auto &english : englishList) {
    // 在单词表里面找到单词，则添加成功。
    auto word_id = checkAlreadyInWords(english);
    if(word_id != -1) {
      legalWords.append(word_id);
    }
  }

  // 如果添加task成功，则返回task_id，否则返回-1代表添加不成功。
  if(legalWords.size() > 0) {
    for(auto word_id :legalWords) {
      auto tmp = createTaskWord(task_id, word_id);
      if(tmp.isNull()) {
        qDebug() << "**************** failed to insert into taskwordtable: " << task_id <<" " <<word_id;
      }
    }

    qDebug() << "*********************** 添加成功 ****************************\n";
    return task_id;
  }
  else {
    qDebug() << "*********************** 添加task失败 ****************************\n";
    return -1;
  }
}

//====================================== Student part =====================================//
//--------------------------- bind parameters --------------------------//
auto db::Student::addStudent(QSqlQuery &q,
                             const qint64 &id,
                             const QString &name,
                             const QString &password,
                             const QString &profile_photo_url) -> QVariant {
  q.addBindValue(id);
  q.addBindValue(name);
  q.addBindValue(password);
  q.addBindValue(profile_photo_url);
  q.exec();

  return q.lastInsertId();
}

auto db::Student::deleteStudent(QSqlQuery &q, const qint64 &id) -> bool {
  q.addBindValue(id);
  return q.exec();
}

auto db::Student::addStudentClass(QSqlQuery &q,
                                  const qint64 &student_id,
                                  const qint64 &class_id) -> QVariant {
  q.addBindValue(student_id);
  q.addBindValue(class_id);
  q.addBindValue(class_id);
  if (!q.exec()) {
    qDebug() << "Error inserting data:" << q.lastError().text();
    return QVariant(); // Return an empty QVariant or handle the error as needed
  }
  return q.lastInsertId();
}

void db::Student::displayStudent(QSqlQuery &q, const qint64 &id) {
  q.addBindValue(id);
  if (!q.exec()) {
    qWarning() << "Query failed:" << q.lastError().text();
  } else {
    qDebug() <<"------ 打印学生信息 -----\n";
    while (q.next()) {
      // 获取每一列的值
      qint64 id = q.value("id").toLongLong();
      QString name = q.value("name").toString();
      QString profilePhotoUrl = q.value("profile_photo_url").toString();

      qDebug() << "id: " << id << "name: " << name << "profilePhotoUrl: " << profilePhotoUrl << "\n";
    }
    qDebug() <<"------ 结束打印学生信息 -----\n";
  }
}

auto db::Student::displayStudentClass(QSqlQuery &q, const qint64 &student_id) ->QList<QPair<qint64, QString>> {
  QList<QPair<qint64, QString>> classesList;

  q.addBindValue(student_id);
  if (!q.exec()) {
    qDebug() << "Error executing displayStudentClassquery:" << q.lastError().text();
    return classesList; // 返回空列表
  }
  while (q.next()) {
    qint64 classId = q.value("id").toLongLong();
    QString className = q.value("name").toString();
    classesList.append(qMakePair(classId, className));
  }

  return classesList;
}

auto db::Student::deleteStudentClass(QSqlQuery &q, const qint64 &student_id, const qint64 &class_id) -> bool {
  q.addBindValue(student_id);
  q.addBindValue(class_id);
  if (!q.exec()) {
    qDebug() << "Delete operation failed:" << q.lastError().text();
    return false;  // 删除失败返回false
  }
  else {
    return true;
  }
}

auto db::Student::insertStudentWordLearningTable(QSqlQuery &q, const qint64 &student_id, const qint64 &word_id) -> QVariant {
  q.addBindValue(student_id);
  q.addBindValue(word_id);
  if (!q.exec()) {
    qDebug() << "insertStudentWordLearningTable:" << q.lastError().text();
  }
  return q.lastInsertId();
}

QVariant db::Student::addSysWord(QSqlQuery &q, const qint64 &student_id, const qint64 &word_id)
{
  q.addBindValue(student_id);
  q.addBindValue(word_id);
  if (!q.exec()) {
    qDebug() << "insertStudentSysLearning:" << q.lastError().text();
  }
  return q.lastInsertId();
}

auto db::Student::addWordBank(QSqlQuery &q, const qint64 &student_id, const qint64 &word_bank_id) -> QVariant
{
  q.addBindValue(student_id);
  q.addBindValue(word_bank_id);
  if (!q.exec()) {
    qDebug() << "addWordBank:" << q.lastError().text();
  }
  return q.lastInsertId();
}

//--------------------------- semantic functions --------------------------//
// 增删改查
auto db::Student::registerRole() -> QVariant {
  // 初始化query。

  QSqlQuery query(returnDatabase());
  if(!query.prepare(insertStudentTable)) {
    throw std::runtime_error("Failed to prepare student register sql");
  }

  return addStudent(query, GetId(), GetName(), GetPassword(), GetProfilePhotoUrl());
}

auto db::Student::cancelRole() ->void {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(deleteStudentRole)) {
    throw std::runtime_error("Failed to prepare TearcherTable delete sql");
  }

  if(!deleteStudent(query, GetId())) {
    qWarning() << "Failed to canel Student: " << GetId() << "\n";
  }
}

auto db::Student::displayInfo() ->void {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(dispalyStduentRole)) {
    throw std::runtime_error("Failed to prepare TearcherTable display sql");
  }

  displayStudent(query, GetId());
}

// 扩展操作
// 学生加入班级，只需要单纯的将studnet_id和class_id插入表格即可。
auto db::Student::joinClass(const qint64 &class_id) -> QVariant {
  // @todo 在老师class表格中插入条目。
  QSqlQuery query(returnDatabase());
  if(!query.prepare(insertStudentClassTable)) {
    throw std::runtime_error("Failed to prepare TearcherClassTable insert sql");
  }
  auto a = addStudentClass(query, GetId(), class_id);
  return a;
}

auto db::Student::leaveClass(const qint64 &class_id) -> bool {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(deleteStudentFromClass)) {
    throw std::runtime_error("Failed to prepare student leave class sql");
  }
  if(!deleteStudentClass(query, GetId(), class_id)) {
    qWarning() << "Student failed to leave class: " << GetId() << "\n";
    return false;
  }

  return true;
}

auto db::Student::infoStudentClass() -> QList<QPair<qint64, QString>> {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(retrieveStudentClasses)) {
    throw std::runtime_error("Failed to prepare student info class sql");
  }
  QList<QPair<qint64, QString>> classList = displayStudentClass(query, GetId());

  qDebug() <<"**************\n";
  for (const auto &pair : classList) {
    qDebug() << "Class ID:" << pair.first << ", Class Name:" << pair.second;
  }
  qDebug() <<"**************\n";

  return classList;
}

auto db::Student::learnWordRecord(const qint64 &word_id) -> QVariant {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(learnWord)) {
    throw std::runtime_error("Failed to prepare retrieveWordFromTask sql");
  }

  return insertStudentWordLearningTable(query, GetId(), word_id);
}

QVariant db::Student::learnSysWordRecord(const qint64 &word_id)
{
  QSqlQuery query(returnDatabase());
  if(!query.prepare(learnSysWords)) {
    throw std::runtime_error("Failed to prepare learnSysWordRecord sql");
  }

  return addSysWord(query, GetId(), word_id);
}

auto db::Student::learnWordBanks(const qint64 &word_bank_id) ->QVariant
{
  QSqlQuery query(returnDatabase());
  if(!query.prepare(learnWordBank)) {
    throw std::runtime_error("Failed to prepare learWordBanks sql");
  }

  return addWordBank(query, GetId(), word_bank_id);
}

auto db::Student::returnStudentBank() -> QVariant
{
  QSqlQuery query(returnDatabase());
  if(!query.prepare(returnStudentWordBank)) {
    throw std::runtime_error("Failed to prepare returnStudentBank sql");
  }

  query.addBindValue(GetId());
  QVariant tmp;
  if (!query.exec()) {
    qDebug() << "Error executing returnStudentWordBank:" << query.lastError().text();
    return tmp;
  }

  if (query.next()) {
    return query.value("word_bank_id");
  } else {
    return tmp;
  }
}

auto db::Teacher::checkAlreadyInWords(const QString &word) -> int {
  QSqlQuery query(returnDatabase());
  if(!query.prepare(wordIdInWords)) {
    throw std::runtime_error("Failed to prepare teacher wordInWords sql");
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




