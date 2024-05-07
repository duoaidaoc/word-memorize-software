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
  QDateTime create_time;
  QDateTime deadline;
  QTime time;
};
} // end namespace db

#endif  // UTIL_H
