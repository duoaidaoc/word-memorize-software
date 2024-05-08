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
  // 设置成选择题，传入英文，a选项，b选项，c选项，正确的选项序号(1表示a，2表示b，3表示c)
  void set_selection_question(const QString &english, const QString &A, const QString &B, const QString &C, int _correctitem);
  // 设置成填空题, 传入中文，正确的英文
  void set_fillword_question(const QString &english, const QString &chinese);
protected:
  void paintEvent(QPaintEvent* evt)override;
private:
  Ui::question *ui;
  QImage* bg;
  int correctitem;
  QString correct_english;
  void set_connection();

signals:
  // 回答完毕时触发这个信号，true表示答对，false表示答错。
  void done(bool);
};

#endif // QUESTION_H
