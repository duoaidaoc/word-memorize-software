#include <qevent.h>
#include "task_frame.h"

task_frame::task_frame(QWidget* parent): QFrame(parent)
{
    hb =new QHBoxLayout(this);
    taskinfo = new QLabel(this);
    taskinfo->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    taskinfo->setWordWrap(true);
    taskinfo->setText(QString("taskname"));
    hb->addWidget(taskinfo);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

void task_frame::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
      emit set_display_content(tsk);
    }
}

