#include <QApplication>
#include "QFile"
#include "QFontDatabase"
#include "role.h"

QFont* glob_font;
QString glob_login_bg_path;
QString glob_stylecss_path;
QString glob_hello;
std::vector<QWidget*>MainWidgets;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // database test
    // 创建数据库对象
    qDebug() << "======== 创建数据库 ========\n";
    db::Database database("wordmemorize.db");

    qDebug() << "======== 创建教师信息 ========\n";
    db::Teacher teacher(database);
    //teacher.SetName("John");
    //teacher.SetPassword("abcdefg");
    //teacher.SetProfilePhotoUrl("djdjdjjdjd");
    //if (!teacher.registerRole())
    //    qDebug() << "Failed to register teacher role";

    qDebug() << "======== 创建学生信息 ========\n";
    // 创建学生对象并注册
    db::Student student(database);
    //student.SetName("Alice");
    //student.SetPassword("djdjdjjdjddddddd");
    //student.SetProfilePhotoUrl("eeueueuL");
    //if (!student.registerRole())
    //    qDebug() << "Failed to register student role";

    qDebug() << "======== 成功结束 ========\n";
}
