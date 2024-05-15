#ifndef TEACHER_MAIN_H
  #define TEACHER_MAIN_H

#include "class_frame.h"
#include "newclass_frame.h"
#include "qboxlayout.h"
#include "qstandarditemModel.h"
#include "settings.h"
#include "task_frame.h"
#include "word_frame.h"
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

  void data_setup();
protected:
  void paintEvent(QPaintEvent* evt)override;

private:
  Ui::teacher_main *ui;


  QVBoxLayout *word_layout,*class_layout,*task_layout,*task_view_layout;
  std::vector<word_frame* >word_frames,task_view_frames;
  std::vector<class_frame* >class_frames;
  std::vector<task_frame* >task_frames;

  QVBoxLayout* student_fixed_layout;
  QHBoxLayout* student_appending_layout[2];

  QImage img;
  newclass_frame* nc_frame;
  CClass current_cls;
  int task_id;

  tip *Tip;

  QImage *profile_pic;

  Settings* settings;

  QString profile_pic_url;

  QStandardItemModel *model,*stu_model;
  void ui_setup();
  void connection_setup();
  void update_class();
  void clearlayout(QBoxLayout *layout);
  void clearNowTask();
  void setNowTask();
};

#endif // TEACHER_MAIN_H
