#ifndef TEACHER_MAIN_H
  #define TEACHER_MAIN_H

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

private:
  Ui::teacher_main *ui;
};

#endif // TEACHER_MAIN_H
