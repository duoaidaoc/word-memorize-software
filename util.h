#ifndef UTIL_H
#define UTIL_H

#include "qdatetime.h"
#include <QString>
#include <QVariant>

// 可能是保温传输用的。

namespace db {
struct TeacherInfo {
  qint64 teacherId;
  QString teacherName;
  QString teacherUrl;
};

struct StudentInfo {
  qint64 studentId;
  QString studentName;
  QString studentUrl;
};

struct TaskInfo {
  qint64 taskId;
  QString taskName;
  QDateTime create_time;
  QDateTime deadline;
  QTime time;
};

struct WordInfo {
  qint64 word_id;
  QString english;
  QString chinese;
  QString phonetic;
  QString audio_url;
};

struct WordBankInfo {
  qint64 id;
  QString name;
  QString picture_url;
};
} // end namespace db

#endif  // UTIL_H
