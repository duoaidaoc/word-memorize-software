#ifndef TIP_H
  #define TIP_H

  #include <QWidget>

namespace Ui {
class tip;
}

class tip : public QWidget
{
  Q_OBJECT

public:
  explicit tip(QWidget *parent = nullptr);
  ~tip();
  void set_content(const QString &title,const QString &content,const QString &Ri = "取消",const QString &Le = "确认");
  qint64 tid;
protected:
  void paintEvent(QPaintEvent *evt);
private:
  Ui::tip *ui;
  QImage bg;

signals:
  void cancel();
  void confirm();
};

#endif // TIP_H
