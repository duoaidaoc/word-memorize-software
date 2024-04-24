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
  // database test
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

  qDebug() << "-------- 创建学生信息 --------\n";
  // 创建学生对象并注册
  db::Student student(database);
  student.SetId(4);
  student.SetName("Alice");
  student.SetPassword("djdjdjjdjddddddd");
  student.SetProfilePhotoUrl("eeueueuL");
  student.registerRole();

  qDebug() << "-------- 数据库测试结束 ========\n";

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
