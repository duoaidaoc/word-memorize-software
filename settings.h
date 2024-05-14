#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include "word.h"
namespace Ui {
class Settings;
}

class Settings: public QWidget
{
  Q_OBJECT

public:
  explicit Settings(QWidget *parent = nullptr);
  ~Settings();

  void get_info(const detailed_info &info);
protected:
  void paintEvent(QPaintEvent* evt)override;

private:
  Ui::Settings *ui;

  QImage *bg;
  detailed_info info_;
  QImage *loaded_img;
  void setup_ui();
  void setup_connection();
  void reset_info();

signals:
  void set_outer_info(detailed_info info);
};

#endif  // SETTINGS_H
