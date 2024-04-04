
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "qdir.h"
#include <QFont>
#include <QString>
#include <QRandomGenerator>

extern QFont* glob_font;
extern QString glob_bg_path;
extern QString glob_hello;
extern QString glob_profile_picture_path;
extern QString glob_audio_path;


class resource_manager
{
public:

    resource_manager(resource_manager const&) = delete;
    void operator=(resource_manager const&) = delete;
    static resource_manager* getInstance();
    QString bg_pic_randomselect();
    QString pf_pic_randomselect();
    QString audio_select(const QString &eng);

private:
    resource_manager();
    static resource_manager* instance;
    QString randomselect(QString path);

};


#endif // RESOURCE_MANAGER_H
