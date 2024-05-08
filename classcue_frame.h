#ifndef CLASSCUE_FRAME_H
  #define CLASSCUE_FRAME_H

#include <QWidget>
#include "tip.h"
namespace Ui {
class classcue_frame;
}

class classcue_frame : public QWidget
{
  Q_OBJECT

public:
  explicit classcue_frame(QWidget *parent = nullptr);
  ~classcue_frame();
protected:
  void paintEvent(QPaintEvent* evt)override;
private:
  Ui::classcue_frame *ui;
  QImage bg;
  tip* Tip;
signals:
  void UpdateClass();
};

#endif // CLASSCUE_FRAME_H
