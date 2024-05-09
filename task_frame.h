
#ifndef TASK_FRAME_H
  #define TASK_FRAME_H

#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include "util.h"

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
  db::TaskInfo tsk;
  void settask(const db::TaskInfo& _tsk);
signals:
  void set_display_content(db::TaskInfo _tsk);
};

#endif // TASK_FRAME_H
