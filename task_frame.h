
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
private:
  QLabel* taskinfo;
  QHBoxLayout* hb;
};

#endif // TASK_FRAME_H
