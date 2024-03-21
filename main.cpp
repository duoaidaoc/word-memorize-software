#include "login.h"
#include "appinit.h"
#include <QApplication>
#include "QFile"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file("../database_crouse_design/settings.css");

    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        qApp->setStyleSheet(qss);
        file.close();
    }

    QFont font("Arial", 10);
    a.setFont(font);

    AppInit::Instance()->start();


    login loginpage;
    loginpage.show();
    return a.exec();
}
