
#include "class_frame.h"
#include "qevent.h"

class_frame::class_frame(QWidget *parent): QFrame(parent)
{
    hb =new QHBoxLayout(this);
    classinfo = new QLabel(this);
    classinfo->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    classinfo->setWordWrap(true);
    classinfo->setText(QString("english class 1"));
    hb->addWidget(classinfo);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

class_frame::~class_frame()
{
    for(auto con : connections){
      QObject::disconnect(con);
    }
}

void class_frame::setclass(CClass &_cls)
{
    cls = _cls;
    // id : name
    classinfo->setText(QString("%1 : %2").arg(cls.id).arg(cls.name));
}

void class_frame::add_connect(QMetaObject::Connection con)
{
    connections.push_back(con);
}

void class_frame::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
      emit set_display_content(cls);
    }
}

