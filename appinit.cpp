#include "appinit.h"
#include "qmutex.h"
#include "qapplication.h"
#include "qevent.h"
#include "qwidget.h"
#include "student_main.h"

AppInit *AppInit::self = 0;
AppInit *AppInit::Instance()
{
    if(!self){
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if(!self){
            self = new AppInit;
        }
    }
    return self;
}

AppInit::AppInit(QObject *parent)
    : QObject{parent}
{

}

bool AppInit::eventFilter(QObject *obj, QEvent *evt)
{
    QWidget *w = (QWidget *)obj;
    if (w->property("canMove").toBool()) {
        static QPoint mousePoint;
        static bool mousePressed = false;

        QMouseEvent *event = static_cast<QMouseEvent *>(evt);
        if (event->type() == QEvent::MouseButtonPress) {
            if (event->button() == Qt::LeftButton) {
                mousePressed = true;
                mousePoint = event->globalPos() - w->pos();
                return true;
            }
        } else if (event->type() == QEvent::MouseButtonRelease) {
            mousePressed = false;
            return true;
        } else if (event->type() == QEvent::MouseMove) {
            if (mousePressed && (event->buttons() && Qt::LeftButton)) {
                w->move(event->globalPos() - mousePoint);
                if(w->objectName() == "student_main"){
                  ((student_main*) w)->movedisplay();
                }
                return true;
            }
        }
    }
    if(w->property("").toBool()){

    }

    return QObject::eventFilter(obj, evt);
}

void AppInit::start()
{
    qApp->installEventFilter(this);
}