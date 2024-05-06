#ifndef UTIL_H
#define UTIL_H

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
} // end namespace db

#endif  // UTIL_H
