#include "login.h"
#include "appinit.h"
#include <QApplication>
#include "QFile"
#include "student_main.h"
#include <QDir>
#include "resource_manager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
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
