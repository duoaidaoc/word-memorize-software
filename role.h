#ifndef ROLE_H
#define ROLE_H

#include "table.h"
#include <QString>
#include <QVariant>
#include "class.h"
#include "task.h"
#include "words.h"
#include "util.h"
//#include "resource_manager.h""

namespace db {
class Role : public Table {
  class ResourceManager;

private:
  qint64 id_;
  QString name_;
  QString password_;
  QString profile_photo_url_;

  const QLatin1String retrieveTaskInfo = QLatin1String(R"(
        SELECT tasktable.id, tasktable.task_name, tasktable.create_time, tasktable.deadline, tasktable.time_limit
        FROM tasktable
        INNER JOIN AssignmentDistributionTable ON tasktable.id = AssignmentDistributionTable.task_id
        WHERE AssignmentDistributionTable.class_id = ?
    )");
  const QLatin1String retrieveWordFromTask = QLatin1String(R"(
        SELECT words.id, words.english, words.chinese, words.phonetic, words.audio_url
        FROM words
        INNER JOIN TaskWordTable ON words.id = TaskWordTable.word_id
        WHERE TaskWordTable.task_id = ?
    )");
  const QLatin1String retrieveTeachersInClass = QLatin1String(R"(
        SELECT teachers.id, teachers.name, teachers.profile_photo_url
        FROM teachers
        INNER JOIN teacherclass ON teachers.id = teacherclass.teacher_id
        WHERE teacherclass.class_id = ?
    )");
  const QLatin1String retrieveClassMember = QLatin1String(R"(
        SELECT students.id, students.name, students.profile_photo_url
        FROM students
        INNER JOIN studentclass ON students.id = studentclass.student_id
        WHERE studentclass.class_id = ?
    )");
  const QLatin1String getTaskWords = QLatin1String(R"(
        SELECT words.id, words.english, words.chinese, words.phonetic, words.audio_url
        FROM TaskWordTable
        JOIN words ON TaskWordTable.word_id = words.id
        WHERE TaskWordTable.task_id = ?
    )");
  const QLatin1String isWordLearnedYet = QLatin1String(R"(
        SELECT DISTINCT student_id
        FROM StudentWordLearning
        WHERE word_id = ? AND student_id = ?
    )");

  static QList<TaskInfo> displayTaskInClass(QSqlQuery &q, const qint64 &class_id);
  static QList<WordInfo> displayWordFromTask(QSqlQuery &q, const qint64 &task_id);
  static QList<TeacherInfo> displayClassTeacher(QSqlQuery &q, const qint64 &class_id);
  static QList<StudentInfo> displayClassMember(QSqlQuery &q, const qint64 &class_id);
  static QList<WordInfo> calculateTotal(QSqlQuery &q, const qint64& task_id);
  static bool isWordLearned(QSqlQuery &q, const WordInfo &wordInfo, const qint64 &student_id);
public:
  explicit Role(Database& db) : Table(db) { }
  virtual QVariant registerRole() = 0;
  virtual void cancelRole() = 0;
  virtual void displayInfo() = 0;

  void SetId(const qint64& id) { id_ = id; }
  void SetName(const QString& name) { name_ = name; }
  void SetPassword(const QString& password) { password_ = password; }
  void SetProfilePhotoUrl(const QString& url) { profile_photo_url_ = url; }

  const qint64 GetId() const { return id_; }
  const QString GetName() const { return name_; }
  const QString GetPassword() const { return password_; }
  const QString GetProfilePhotoUrl() const { return profile_photo_url_; }
  QList<TaskInfo> infoTaskInClass(const qint64 &class_id);
  QList<WordInfo> infoWordsInTask(const qint64 &task_id);
  QList<TeacherInfo> infoClassDetails(const qint64 &class_id);
  QList<StudentInfo> infoClassMembers(const qint64 &class_id);

  double infoTaskCondition(const qint64 &studnet_id, const qint64 &task_id);
};

class Teacher : public Role {

private:
  const QLatin1String insertTeacherTable = QLatin1String(R"(
        INSERT INTO teachers(id, name, password, profile_photo_url) VALUES(?, ?, ?, ?)
    )");
  const QLatin1String deleteTeacherRole = QLatin1String(R"(
        DELETE FROM teachers WHERE id = ?
    )");
  const QLatin1String dispalyTeacherRole = QLatin1String(R"(
        SELECT * FROM teachers WHERE id = ?
    )");
  const QLatin1String insertTeacherClassTable = QLatin1String(R"(
        INSERT INTO teacherclass(teacher_id, class_id) VALUES(?, ?)
    )");
  const QLatin1String teacherDeleteClass = QLatin1String(R"(
        DELETE FROM class WHERE id = ?
    )");
  const QLatin1String teacherDeleteTeacherClass = QLatin1String(R"(
        DELETE FROM teacherclass WHERE teacher_id = ? AND class_id = ?
    )");
  const QLatin1String teacherDeleteStudentClass = QLatin1String(R"(
        DELETE FROM studentclass WHERE class_id = ?
    )");
  const QLatin1String insertAssignmentDistribution = QLatin1String(R"(
        INSERT INTO AssignmentDistributionTable(teacher_id, task_id, class_id) VALUES(?, ?, ?)
    )");
  const QLatin1String teacherDeleteAssignmentDistribution = QLatin1String(R"(
        DELETE FROM AssignmentDistributionTable WHERE teacher_id = ? AND task_id = ? AND class_id = ?
    )");
  const QLatin1String teacherDeleteTask = QLatin1String(R"(
        DELETE FROM tasktable WHERE id = ?
    )");
  const QLatin1String insertTaskWordTable = QLatin1String(R"(
        INSERT INTO TaskWordTable(task_id, word_id) VALUES(?, ?)
    )");
  const QLatin1String retrieveTeacherClasses = QLatin1String(R"(
        SELECT class.id AS id, class.name AS name
        FROM teacherclass JOIN class ON teacherclass.class_id = class.id
        WHERE teacherclass.teacher_id = ?
    )");
  const QLatin1String wordIdInWords = QLatin1String(R"(
        SELECT id FROM words WHERE english = ?
    )");

  // 教师语义操作
  static QVariant addTeacher(QSqlQuery &q, const qint64 &id, const QString &name, const QString &password, const QString &profile_photo_url);
  static bool deleteTeacher(QSqlQuery &q, const qint64 &id);
  static void displayTeacher(QSqlQuery &q, const qint64 &id);
  static QVariant addTeacherClass(QSqlQuery &q, const qint64 &teacher_id, const qint64 &class_id);
  static bool teacherDeleteClassTable(QSqlQuery &q, const qint64 &class_id);
  static int teacherDeleteTeacherClassTable(QSqlQuery &q, const qint64 &teacher_id, const qint64 &class_id);
  static bool teacherDeleteStudentClassTable(QSqlQuery &q, const qint64 &class_id);
  static QVariant addTeacherTaskClass(QSqlQuery &q, const qint64 &teacher_id, const qint64 &task_id, const qint64 &class_id);
  static int teacherDeleteAssignmentDistributionTable(QSqlQuery &q, const qint64 &teacher_id, const qint64 &task_id, const qint64 &class_id);
  static bool teacherDeleteTaskTable(QSqlQuery &q, const qint64 &task_id);
  static QVariant addTaskWord(QSqlQuery &q, const qint64 &task_id, const qint64 &word_id);
  static QList<QPair<qint64, QString>> displayTeacherClass(QSqlQuery &q, const qint64 &student_id);
  int returnTaskNumber();

public:
  explicit Teacher(Database& db) : Role(db) { }

  // 增删改查
  QVariant registerRole() override;
  void cancelRole() override;
  void displayInfo() override;

  // 扩展操作
  // 老师创建班级需要：Class_id, Class_name, Class_cue
  QVariant createClass(const qint64 &class_id, const QString &class_name, const QString &class_cue);
  // 老师删除班级只需要：Class_id
  bool deleteClass(const qint64 &class_id);
  // 老师创建任务：需要和班级绑定。
  QVariant createTask(const qint64 &task_id, const qint64 &class_id, const QString& task_name, const QDateTime &create_time, const QDateTime &deadline, const QTime &time_limit);
  // 老师删除班级里面的人物。
  bool deleteTask(const qint64 &task_id, const qint64 &class_id);
  // 老师给任务添加单词
  QVariant createTaskWord(const qint64 &task_id, const qint64 &word_id);
  QList<QPair<qint64, QString> > infoTeacherClass();
  // 返回系统分配的task_id
  int importTaskWordBank(const QList<QString> &englishList);
  int checkAlreadyInWords(const QString &word);

  // 查看系统的task_id到哪个值了。
  int getTaskId();
  void storeImageForStudent(const QString &filePath, qint64 studentId);
  QString retrieveImageForStudent(qint64 teacherId);
};

class Student : public Role {
private:
  const QLatin1String insertStudentTable = QLatin1String(R"(
        INSERT INTO students(id, name, password, profile_photo_url, plan) VALUES(?, ?, ?, ?, ?)
    )");
  const QLatin1String deleteStudentRole = QLatin1String(R"(
        DELETE FROM students WHERE id = ?
    )");
  const QLatin1String dispalyStduentRole = QLatin1String(R"(
        SELECT * FROM students WHERE id = ?
    )");
  const QLatin1String insertStudentClassTable = QLatin1String(R"(
        INSERT INTO studentclass(student_id, class_id) VALUES(?, ?)
    )");
  const QLatin1String returnClassId = QLatin1String(R"(
        SELECT id FROM class
        WHERE id = ?
    )");
  const QLatin1String deleteStudentFromClass = QLatin1String(R"(
        DELETE FROM studentclass WHERE student_id = ? AND class_id = ?
    )");
  const QLatin1String retrieveStudentClasses = QLatin1String(R"(
        SELECT class.id AS id, class.name AS name
        FROM studentclass JOIN class ON studentclass.class_id = class.id
        WHERE studentclass.student_id = ?
    )");
  const QLatin1String learnWord = QLatin1String(R"(
        INSERT INTO StudentWordLearning(student_id ,word_id) VALUES(?, ?)
    )");
  const QLatin1String learnSysWords = QLatin1String(R"(
        INSERT INTO StudentSysLearn(student_id, word_id) VALUES(?, ?)
    )");

  // TODO马方舟: change this sqlite sql into mysql version.
  const QLatin1String learnWordBank = QLatin1String(R"(
        REPLACE INTO createStudentLearnWordBank (student_id, word_bank_id) VALUES (?, ?)
    )");

  const QLatin1String returnStudentWordBank = QLatin1String(R"(
        SELECT word_bank_id FROM createStudentLearnWordBank WHERE student_id = ?
    )");
  const QLatin1String infoStudentPlan = QLatin1String(R"(
        SELECT students.plan FROM students WHERE id = ?
    )");
  const QLatin1String updateStudentPlan = QLatin1String(R"(
        UPDATE students
        SET plan = ?
        WHERE id = ?
    )");
  const QLatin1String insertRanking = QLatin1String(R"(
        INSERT INTO rankings (id, score, nickname)
        VALUES (:id, :score, :nickname)
        ON DUPLICATE KEY UPDATE
            score = IF(VALUES(score) > score, VALUES(score), score),
            nickname = IF(VALUES(nickname) != nickname, VALUES(nickname), nickname);
    )");

  // 学生语义操作
  static QVariant addStudent(QSqlQuery &q, const qint64 &id, const QString &name, const QString &password, const QString &profile_photo_url, const qint64 &plan);
  static bool deleteStudent(QSqlQuery &q, const qint64 &id);
  static void displayStudent(QSqlQuery &q, const qint64 &id);
  static QVariant addStudentClass(QSqlQuery &q, const qint64 &student_id, const qint64 &class_id);
  static bool deleteStudentClass(QSqlQuery &q, const qint64 &student_id, const qint64 &class_id);
  static QList<QPair<qint64, QString>> displayStudentClass(QSqlQuery &q, const qint64 &student_id);
  static QVariant insertStudentWordLearningTable(QSqlQuery &q, const qint64 &student_id, const qint64 &word_id);
  static QVariant addSysWord(QSqlQuery &q, const qint64 &student_id, const qint64 &word_id);
  static QVariant addWordBank(QSqlQuery &q, const qint64 &student_id, const qint64 &word_bank_id);

public:
  explicit Student(Database& db) : Role(db) { }

  // 增删改查
  QVariant registerRole() override;
  void cancelRole() override;
  void displayInfo() override;

  // 扩展操作
  QVariant joinClass(const qint64 &class_id);
  bool leaveClass(const qint64 &class_id);

  QList<QPair<qint64, QString>> infoStudentClass();
  QVariant learnWordRecord(const qint64 &word_id);
  QVariant learnSysWordRecord(const qint64 &word_id);

  QVariant learnWordBanks(const qint64 &word_bank_id);
  QVariant returnStudentBank();

  QVariant infoPlan();
  void updatePlan(const qint64 &plan);

  bool isClassExit(const qint64 &class_id);
  QVariant recordRanking(const qint64 &score, const QString &nickname);
  RankingInfo returnRanking(const qint64 &student_id);

  void storeImageForStudent(const QString &filePath, qint64 studentId);
  QString retrieveImageForStudent(qint64 student_id);
};
} // end db

#endif // ROLE_H
