#ifndef GAME_H
#define GAME_H

#include "qboxlayout.h"
#include "tip.h"
#include "util.h"
#include <QWidget>

namespace Ui {
class game;
}

class game: public QWidget
{
  Q_OBJECT

public:
  explicit game(QWidget *parent = nullptr);
  ~game();

protected:
  void paintEvent(QPaintEvent* evt)override;

private:
  Ui::game *ui;

  QImage* bg;
  QTimer* timer;


  QList<db::WordInfo>words;
  QList<std::pair<int, QString>>rank;

  QVBoxLayout* layout;

  tip* Tip;
  QString name;

  int idx = 0;
  int initialTime = 10;
  int nowTime = 10;
  int now_score = 0;

  void setup_ui();
  void setup_connection();
  void setup_words();
  void set_play_page();
  void clearlayout(QBoxLayout* lo);

};

#endif  // GAME_H
