#ifndef QUESTION_H
  #define QUESTION_H

  #include <QWidget>

namespace Ui {
class question;
}

class question : public QWidget
{
  Q_OBJECT

public:
  explicit question(QWidget *parent = nullptr);
  ~question();
protected:
  void paintEvent(QPaintEvent* evt)override;
private:
  Ui::question *ui;
  QImage* bg;
};

#endif // QUESTION_H
