#ifndef TEACHER_MAIN_H
  #define TEACHER_MAIN_H

#include "qboxlayout.h"
#include <QWidget>

namespace Ui {
class teacher_main;
}

class teacher_main : public QWidget
{
  Q_OBJECT

public:
  explicit teacher_main(QWidget *parent = nullptr);
  ~teacher_main();
protected:
  void paintEvent(QPaintEvent* evt)override;

private:
  Ui::teacher_main *ui;

  QVBoxLayout* class_layout, *task_layout;

  QImage img;
  void ui_setup();
  void connection_setup();
  void data_setup();
  void test();
};

#endif // TEACHER_MAIN_H
