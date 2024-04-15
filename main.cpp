#include "login.h"
#include "appinit.h"
#include <QApplication>
#include "QFile"
#include "student_main.h"
#include <QDir>
#include "resource_manager.h"
QFont glob_font;
QString glob_bg_path;
QString glob_stylecss_path;
QString glob_hello;
QString glob_profile_picture_path = "../database_crouse_design/pics/login_label";
QString glob_audio_path = "../database_crouse_design/audios";
word_display* glob_display;


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

    AppInit::Instance()->start();

    glob_display = new word_display();
    login* loginpage = new login();
    // loginpage->show();
    student_main* stm = new student_main();
    stm->show();
    stm->setfollower(glob_display);

    return a.exec();
}
