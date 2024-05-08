
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "qdir.h"
#include <QFont>
#include <QString>
#include <QRandomGenerator>
#include "QFontDatabase"
#include <QColor>

#include "database.h"
#include "role.h"

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

    void init_database();
    db::Database &get_database();

    db::Student &get_student();
    db::Teacher &get_teacher();

    QVariant init_student(const qint64 id, const QString name, const QString password);
    QVariant init_teacher(const qint64 id, const QString name, const QString password);
    void init_init_student();
    void init_init_teacher();
    void destroy_student();
    void destroy_teacher();
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

    db::Database *database;
    db::Student *student;
    db::Teacher *teacher;
    int sel;
};


#endif // RESOURCE_MANAGER_H
