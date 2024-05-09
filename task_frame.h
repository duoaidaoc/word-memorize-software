
#ifndef TASK_FRAME_H
  #define TASK_FRAME_H

#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include "util.h"
#include "word.h"

class task_frame: public QFrame
{
  Q_OBJECT
public:
  task_frame(QWidget* parent = nullptr);
  void settask(const CClass &cls, const db::TaskInfo& _tsk);
protected:
  void mousePressEvent(QMouseEvent* event)override;
private:
  QLabel* taskinfo;
  QHBoxLayout* hb;
  db::TaskInfo tsk;
  CClass cls_;
signals:
  void set_display_content(db::TaskInfo _tsk);
};

#endif // TASK_FRAME_H
