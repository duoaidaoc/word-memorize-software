#include "login.h"
#include "appinit.h"
#include <QApplication>
#include "QFile"
#include "QFontDatabase"
#include "student_main.h"
#include "qdatetime.h"

QFont* glob_font;
QString glob_bg_path;
QString glob_stylecss_path;
QString glob_hello;
QString glob_profile_picture_path = "../database_crouse_design/pics/login_label";
QString glob_audio_path = "../database_crouse_design/audios";
word_display* glob_display;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTime current_time = QTime::currentTime();
    int sel = current_time.hour();
    sel = 12;
    if (sel < 6 || sel >= 19) {
        //晚上
        glob_stylecss_path = "../database_crouse_design/night.css";
        glob_bg_path = "../database_crouse_design/pics/night_bg";
        glob_hello = "晚上好啊！";
    } else if (sel < 14) {
        //上午
        glob_stylecss_path = "../database_crouse_design/morning.css";
        glob_bg_path = "../database_crouse_design/pics/morning_bg";
        glob_hello = "上午好啊！";
    } else if (sel < 19) {
        //下午
        glob_stylecss_path = "../database_crouse_design/afternoon.css";
        glob_bg_path = "../database_crouse_design/pics/afternoon_bg";
        glob_hello = "下午好啊！";
    }

    QFile dFontFile("../database_crouse_design/ttf/No.233-上首怪兽体.ttf");
    if(!dFontFile.open(QIODevice::ReadOnly))
    {
        //说明打开字体文件失败了
        return false;
    }
    int nFontId = QFontDatabase::addApplicationFontFromData(dFontFile.readAll());
    if(nFontId == -1)
    {
        //说明加载字体文件失败了，该字体不可用
        return false;
    }
    QStringList lFontFamily = QFontDatabase::applicationFontFamilies(nFontId);
    if(lFontFamily.empty())
    {
        //说明从字体中获取字体簇失败了
        return false;
    }
    glob_font = new QFont (lFontFamily.at(0), 15);
    a.setFont(*glob_font);


    QFile file(glob_stylecss_path);
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        qApp->setStyleSheet(qss);
        file.close();
    }

    AppInit::Instance()->start();

    glob_display = new word_display();

    login loginpage;
    loginpage.show();
    student_main stm;
    stm.show();
    stm.setfollower(glob_display);

    return a.exec();
}
