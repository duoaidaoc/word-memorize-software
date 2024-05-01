#include "login.h"
#include "appinit.h"
#include <QApplication>
#include "QFile"
#include "student_main.h"
#include <QDir>
#include "resource_manager.h"
#include "role.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  //========================================== 数据库测试 ==========================================//
  // 创建数据库对象
  qDebug() << "======== 开始数据库测试 ========\n";
  qDebug() << "-------- 创建数据库 --------\n";
  db::Database database("wordmemorize.db");

  qDebug() << "-------- 创建教师信息 --------\n";
  db::Teacher teacher(database);
  teacher.SetId(3);
  teacher.SetName("John");
  teacher.SetPassword("abcdefg");
  teacher.SetProfilePhotoUrl("djdjdjjdjd");
  teacher.registerRole();

  db::Teacher teacher2(database);
  teacher2.SetId(9);
  teacher2.SetName("Alic");
  teacher2.SetPassword("ieieie");
  teacher2.SetProfilePhotoUrl("cncnncnc");
  teacher2.registerRole();

  teacher.cancelRole();
  teacher2.displayInfo();

  qDebug() << "-------- 创建学生信息 --------\n";
  // 创建学生对象并注册
  db::Student student(database);
  student.SetId(4);
  student.SetName("Alice");
  student.SetPassword("djdjdjjdjddddddd");
  student.SetProfilePhotoUrl("eeueueuL");
  student.registerRole();

  db::Student student2(database);
  student2.SetId(89);
  student2.SetName("MIKE");
  student2.SetPassword("eieiiedjjcncn");
  student2.SetProfilePhotoUrl("llwlwl");
  student2.registerRole();

  student.cancelRole();
  student2.displayInfo();

  qDebug() << "-------- 测试老师创建班级 --------\n";
  teacher2.createClass(2023, "english1", "nnnnnnn");

  qDebug() << "-------- 数据库测试结束 -------\n";

  //========================================== 前端测试 ==========================================//
  auto man = resource_manager::getInstance();
  a.setFont(man->get_glob_font());


  QFile file(man->get_glob_stylecss_path());
  if (file.open(QFile::ReadOnly)) {
      QString qss = QLatin1String(file.readAll());
      qApp->setStyleSheet(qss);
      file.close();
  }

  login* loginpage = new login();
  loginpage->show();
  student_main* stm = new student_main();
  stm->show();

  AppInit::Instance()->start();
  return a.exec();
}
