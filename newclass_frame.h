#ifndef NEWCLASS_FRAME_H
  #define NEWCLASS_FRAME_H

#include <QWidget>
#include <word.h>
#include "tip.h"
namespace Ui {
class newclass_frame;
}

class newclass_frame : public QWidget
{
  Q_OBJECT

public:
  explicit newclass_frame(QWidget *parent = nullptr);
  ~newclass_frame();
protected:
  void paintEvent(QPaintEvent* evt)override;
private:
  Ui::newclass_frame *ui;
  QImage bg;
  CClass cls;
  tip *Tip;
signals:
  void UpdateClass();
};

#endif // NEWCLASS_FRAME_H
