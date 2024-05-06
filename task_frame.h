
#ifndef TASK_FRAME_H
  #define TASK_FRAME_H

#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include "word.h"

class task_frame: public QFrame
{
  Q_OBJECT
public:
  task_frame(QWidget* parent = nullptr);
protected:
  void mousePressEvent(QMouseEvent* event)override;
private:
  QLabel* taskinfo;
  QHBoxLayout* hb;
  Task tsk;
signals:
  void set_display_content(Task _tsk);
};

#endif // TASK_FRAME_H
