
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "qdir.h"
#include <QFont>
#include <QString>
#include <QRandomGenerator>
#include "QFontDatabase"
#include <QColor>

class resource_manager
{
public:

    resource_manager(resource_manager const&) = delete;
    void operator=(resource_manager const&) = delete;
    static resource_manager* getInstance();
    QString bg_pic_randomselect();
    QString pf_pic_randomselect();
    QString audio_select(const QString &eng);
    QString get_noise_pic();
    QString get_glob_stylecss_path();
    QString get_glob_bg_path();
    QString get_glob_hello();
    QString get_glob_profile_picture_path();
    QString get_glob_audio_path();

    QFont get_glob_font();
    QColor get_color();

    QColor get_reversed_color();
  private:
    resource_manager();
    static resource_manager* instance;
    QString randomselect(QString path);

    QFont glob_font;
    QString glob_bg_path;
    QString glob_hello;
    QString glob_profile_picture_path;
    QString glob_audio_path;
    QString glob_stylecss_path;

    int sel;
};


#endif // RESOURCE_MANAGER_H
