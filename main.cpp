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
  auto man = resource_manager::getInstance();
//  ========================================== 数据库测试 ==========================================//
//   创建数据库对象
  man->init_database();
  auto &database = man->get_database();
//  qDebug() << "======== 开始数据库测试 ========\n";
//  qDebug() << "-------- 创建数据库 --------\n";

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

  db::Student student3(database);
  student3.SetId(90);
  student3.SetName("Leon");
  student3.SetPassword("eieiiedjjcncn");
  student3.SetProfilePhotoUrl("llwlwl");
  student3.registerRole();
  student3.displayInfo();

  qDebug() << "-------- 测试老师创建班级 --------\n";
  teacher2.createClass(2023, "english1", "nnnnnnn");
  teacher2.createClass(2021, "english2", "mmmmmmm");
  teacher2.createClass(2024, "english3", "mmmmmmm");
  teacher2.createClass(2026, "english4", "mmmmmmm");
  teacher2.createClass(2027, "english5", "mmmmmmm");
  teacher2.createClass(2028, "english6", "mmmmmmm");
  teacher2.createClass(2029, "english7", "mmmmmmm");
  teacher2.createClass(2020, "english8", "mmmmmmm");
  teacher2.createClass(100000, "english10", "mmmmmmm");

  qDebug() << "-------- 测试学生加入班级 --------\n";
  student2.joinClass(2023);                 // todo: 不能重复添加，是在db出解决还是逻辑中解决。
  student2.joinClass(2021);
  student2.joinClass(2020);
  student2.joinClass(2028);
  student2.joinClass(2029);
  student3.joinClass(2020);
  student2.joinClass(2020);               // 会报错，并且导致崩溃

  qDebug() << "-------- 测试老师删除班级 --------\n";
  // 需要支持级联删除。
  // 老师删除对应如下操作：
  // 1. class表格里面删除
  // 2. teacherclass表格里面删除
  // 3. studentclass表格里面删除
  teacher2.deleteClass(2028);

  qDebug() << "-------- 测试学生退出班级 --------\n";
//  student2.leaveClass(2021);
  teacher2.deleteClass(2029);
  teacher.deleteClass(2020);

  qDebug() << "-------- 测试老师给班级添加计划 --------\n";
  teacher2.createTask(2, 2020, QDateTime::currentDateTime(), QDateTime::currentDateTime(), QTime::currentTime());

  qDebug() << "-------- 测试老师给班级去除计划 --------\n";
  teacher2.deleteTask(2, 2020);
  teacher2.createTask(3, 2024, QDateTime::currentDateTime(), QDateTime::currentDateTime(), QTime::currentTime());
  teacher2.createTask(6, 2020, QDateTime::currentDateTime(), QDateTime::currentDateTime(), QTime::currentTime());
  teacher2.createTask(5, 2020, QDateTime::currentDateTime(), QDateTime::currentDateTime(), QTime::currentTime());

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
  // 返回的是该class的学生成员信息。QList<StudentInfo>
  student2.infoClassMembers(2020);

  qDebug() << "-------- 测试学生背任务里面的单词 --------\n";
  // ToDo: （1）班级查任务
  student2.infoTaskInClass(2020);

  // ToDo: （2）显示所有单词
  // ToDo: （3）学生学习单词（单词id）
  // ToDo: （4）显示总体完成情况

  qDebug() << "-------- 测试学生背诵系统单词 --------\n";
  // 流程是系统推送单词（1）选取特定单词本（2）特定单词本选取单词

  qDebug() << "-------- 测试老师查看学习情况 --------\n";
  // ToDo: （1）班级查任务
  // ToDo: （2）显示所有单词
  // ToDo: （3）显示总体完成情况

  qDebug() << "-------- 数据库测试结束 -------\n";


//  //========================================== 前端测试 ==========================================//

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
