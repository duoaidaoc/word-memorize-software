#include "login.h"
#include "appinit.h"
#include <QApplication>
#include "QFile"
#include "student_main.h"
#include <QDir>
#include "resource_manager.h"
#include "role.h"
#include "system.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  //========================================== 数据库测试 ==========================================//
  // 创建数据库对象
  qDebug() << "======== 开始数据库测试 ========\n";
  qDebug() << "-------- 创建数据库 --------\n";
  db::Database database("wordmemorize.db");

  // 初始化服务器系统，服务器端。
  db::System system(database);

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
  teacher2.createClass(2021, "english2", "mmmmmmm");
  teacher2.createClass(2024, "english3", "mmmmmmm");
  teacher2.createClass(2026, "english4", "mmmmmmm");
  teacher2.createClass(2027, "english5", "mmmmmmm");
  teacher2.createClass(2028, "english6", "mmmmmmm");
  teacher2.createClass(2029, "english7", "mmmmmmm");
  teacher2.createClass(2020, "english8", "mmmmmmm");

  qDebug() << "-------- 测试学生加入班级 --------\n";
  student2.joinClass(2023);                 // todo: 不能重复添加，是在db出解决还是逻辑中解决。
  student2.joinClass(2021);
  student2.joinClass(2020);
  student2.joinClass(2028);
  student2.joinClass(2029);
  //student2.joinClass(2020);               // 会报错，并且导致崩溃

  qDebug() << "-------- 测试老师删除班级 --------\n";
  // 需要支持级联删除。
  // 老师删除对应如下操作：
  // 1. class表格里面删除
  // 2. teacherclass表格里面删除
  // 3. studentclass表格里面删除
  teacher2.deleteClass(2028);

  qDebug() << "-------- 测试学生退出班级 --------\n";
  student2.leaveClass(2021);
  teacher2.deleteClass(2029);
  teacher.deleteClass(2020);

  qDebug() << "-------- 测试老师给班级添加计划 --------\n";
  teacher2.createTask(2, 2020, QDateTime::currentDateTime(), QDateTime::currentDateTime(), QTime::currentTime());

  qDebug() << "-------- 测试老师给班级去除计划 --------\n";
  teacher2.deleteTask(2, 2020);
  teacher2.createTask(3, 2024, QDateTime::currentDateTime(), QDateTime::currentDateTime(), QTime::currentTime());

  qDebug() << "-------- 测试老师给计划添加单词 --------\n";
  teacher2.createTaskWord(3, 11, "abandon", "抛弃", "aaaaa", "bbbbb");
  teacher2.createTaskWord(3, 12, "accccc", "抛弃", "aaaaa", "bbbbb");
  teacher2.createTaskWord(3, 13, "accccc", "抛弃", "aaaaa", "bbbbb");
  teacher2.createTaskWord(3, 14, "accccc", "抛弃", "aaaaa", "bbbbb");
  teacher2.createTaskWord(3, 15, "accccc", "抛弃", "aaaaa", "bbbbb");

  qDebug() <<"-------- 测试系统生成自带的题库 --------\n";
  system.createWordBank(1, "牛津", "djdjdjjdjdjdjdj");
  system.importWordBank(1, 16, "oxford", "牛津", "ddddd", "dddddddd");
  system.importWordBank(1, 11, "abandon", "抛弃", "aaaaa", "bbbbb");

  qDebug() << "-------- 测试学生显示所在的班级 --------\n";
  // 返回的是QList<QPair<qint64, QString>>
  student2.infoStudentClass();

  qDebug() << "-------- 测试学生显示所在的班级的详细信息 --------\n";
  // param1: Class ID
  // 返回的是该class的老师信息。QList<TeacherInfo>
  student2.infoClassDetails(2020);
  // 返回的是该class的学生信息。QList<ClassInfo>
  //student2.infoClassMembers(2020);
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

//  login* loginpage = new login();
//  loginpage->show();
  student_main* stm = new student_main();
  stm->show();

  AppInit::Instance()->start();
  return a.exec();
}
