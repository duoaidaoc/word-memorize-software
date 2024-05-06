#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QVariant>

// 可能是保温传输用的。

namespace db {
struct ClassInfo {
  qint64 classId;
  QString className1;
  QString classUrl;
};

struct TeacherInfo {
  qint64 teacherId;
  QString teacherName;
  QString teacherUrl;
};

} // end namespace db

#endif  // UTIL_H
